#include <iostream>
#include <fstream>  
#include <map>
#include <filesystem>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

using namespace std;

string replaceAll(string str, const string& from, const string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

string anilist(int id, string query) {
	json variables = {
		{"id", id}
	};

	json post_bod = {
		{"query", query},
		{"variables", variables}
	};

	cpr::Response r = cpr::Post(cpr::Url{ "https://graphql.anilist.co" },
		cpr::Body(post_bod.dump()),
		cpr::Header{ {"Content-Type", "application/json"} });

	if (r.status_code != 200) throw r.text;
	else return r.text;
}

string getManga(int id) {

	string query = "query ($id: Int) {\
		Media(id: $id, type : MANGA) {\
			id\
			title {\
				romaji\
			}\
			description\
			genres\
			status\
			staff{\
				nodes {\
					id\
				}\
				edges {\
					id\
					role\
				}\
			}\
		}\
	}";

	try {
		string ret = anilist(id, query);
		return ret;
	}
	catch (string e) {
		json error = e;
		return error.dump();
	}

}

string getMangaka(int id) {

	string query = "query ($id: Int) {\
		Staff(id: $id) {\
		id\
			name {\
				full\
			}\
		}\
	}";

	try {
		string ani = anilist(id, query);
		string ret = json::parse(ani)["data"]["Staff"]["name"]["full"];
		return ret;
	}
	catch (string e) {
		json error = e;
		return error.dump();
	}

}

int main() {
	int id;
	cout << "Enter manga ID:\t";
	cin >> id;
	cout << '\n';
	string manga;
	try {
		manga = getManga(id);
	}
	catch (string e) {
		cout << "ERROR: Unable to get manga due to the following error - " << e;
		return 1;
	}

	auto base_info = json::parse(manga)["data"]["Media"];

	string manga_status;
	if (base_info["status"] == "RELEASING") manga_status = "1";
	else if (base_info["status"] == "FINISHED") manga_status = "2";
	else if (base_info["status"] == "NOT_YET_RELEASED") manga_status = "3";
	else  manga_status = "0";

	string author;
	string artist;
	int count = 0;
	for (auto& element : base_info["staff"]["edges"]) {
		if (element["role"] == "Story & Art") {
			string name = getMangaka(base_info["staff"]["nodes"][count]["id"]);

			author = name;
			artist = name;
			break;
		}
		else if (element["role"] == "Story") {
			author = getMangaka(base_info["staff"]["nodes"][count]["id"]);
		}
		else if (element["role"] == "Art") {
			artist = getMangaka(base_info["staff"]["nodes"][count]["id"]);
		}

		if (!author.empty() && !artist.empty()) break;
		count++;
	}

	string desc = base_info["description"];
	desc = replaceAll(desc, "\n<br>", "\n");
	desc = replaceAll(desc, "<br>\n", "\n");
	desc = replaceAll(desc, "<br>", "\n");
	desc = replaceAll(desc, "<i>", "");
	desc = replaceAll(desc, "</i>", "");
	desc = replaceAll(desc, "< !--link-->", "");
	desc = replaceAll(desc, "<b>", "");
	desc = replaceAll(desc, "</b>", "");

	nlohmann::ordered_json output = {
		{"title",base_info["title"]["romaji"]},
		{"author", author},
		{"artist", artist},
		{"description",desc},
		{"genre",base_info["genres"]},
		{"status",manga_status},
		{"_status values",json::array({
			"0 = Unknown",
			"1 = Ongoing",
			"2 = Completed",
			"3 = Licensed"
		})},
		{"_anilist_id", base_info["id"]}
	};

	ofstream outfile("details.json");
	outfile << output.dump(4);
	outfile.close();
	cout << "The file \"details.json\" was created successfully!\nPress Enter to Exit...";
	cin.ignore();
	cin.get();
	return 0;
}
