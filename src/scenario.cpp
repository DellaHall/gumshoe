#include <iostream>
#include <fstream>
#include <string>
#include <json.hpp>
#include <random>
#include <ctime>

using namespace std;
using json = nlohmann::json;

json scenario_gen(int seed = time(nullptr)) {
  auto rng = std::default_random_engine(seed);
  srand(seed);
  json scenario;

  scenario["seed"] = seed;
  // Load dossier
  ifstream in_file("resource/dossier.json");
  json dossier;
  in_file >> dossier;

  // Shuffle the alphabet
  vector<int> alphabet;
  for (int i = 0; i < 26; i++) {
    alphabet.push_back(i);
  }
  shuffle(alphabet.begin(), alphabet.end(), rng);

  // Generate NPCs
  scenario["npcs"] = {};
  for (int i = 0; i < 7; i++) {
    // Pick NPC Gender
    string gender = "X";
    int g = (rand() % 20) + 1;
    if (g >= 1 && g <= 9) { gender = "M"; }
    if (g >= 10 && g <= 18) { gender = "F"; }

    // Pick Pronouns
    vector<string> pronouns = dossier["pronouns"][gender];

    // Name the NPC
    string firstname = dossier[gender][alphabet[i]].get<string>();
    string lastname = dossier["surnames"][alphabet.end()[-(i + 1)]].get<string>();
    // Sometimes make NPCs related
    if (int ((rand()) % 3) == 0) {
      lastname = dossier["surnames"][alphabet.end()[-1]].get<string>();
    }

    // Pick NPC's Saluation
    vector<string> avail_salutes;
    // Always include gender-specific salutations
    avail_salutes.insert(avail_salutes.end(), dossier["salutations"][gender].begin(), dossier["salutations"][gender].end());
    // Sometimes include other salutations
    if (int ((rand()) % 3) == 0) {
      avail_salutes.insert(avail_salutes.end(), dossier["salutations"]["A"].begin(), dossier["salutations"]["A"].end());
    }
    int s = (rand() % avail_salutes.size());
    string salutation = avail_salutes[s];

    // Add NPC to scenario json
    scenario["npcs"][i]["firstname"] = firstname;
    scenario["npcs"][i]["lastname"] = lastname;
    scenario["npcs"][i]["gender"] = gender;
    scenario["npcs"][i]["salutation"] = salutation;
    scenario["npcs"][i]["pronouns"] = pronouns;
    scenario["npcs"][i]["fullname"] = salutation + " " + firstname + " " + lastname;
  }

  // Assign Victim and Murderer values
  // These are static positions, but still want to have them stored
  scenario["npcs"][0]["victim"] = true;
  scenario["npcs"][6]["guilty"] = true;

  // Assign alibi people
  // These are static positions, but still want to have them stored

  scenario["npcs"][1]["alibi"] = 2;
  scenario["npcs"][2]["alibi"] = 1;
  scenario["npcs"][3]["alibi"] = 4;
  scenario["npcs"][4]["alibi"] = 3;
  scenario["npcs"][5]["alibi"] = 5;
  scenario["npcs"][6]["alibi"] = 4;
  

  // Pick Rooms - Need 3 for alibis, 1 for murder
  vector<string> rooms = dossier["rooms"];
  shuffle(rooms.begin(), rooms.end(), rng);
  scenario["npcs"][0]["room"] = rooms[0];
  scenario["npcs"][1]["room"] = rooms[1];
  scenario["npcs"][2]["room"] = rooms[1];
  scenario["npcs"][3]["room"] = rooms[2];
  scenario["npcs"][4]["room"] = rooms[2];
  scenario["npcs"][5]["room"] = rooms[3];
  scenario["npcs"][6]["room"] = rooms[3];

  // Pick the murder weapon
  int w = ((rand()) % dossier["weapons"].size());
  scenario["weapon"] = dossier["weapons"][w][0];
  scenario["damage"] = dossier["weapons"][w][1];

  return scenario;
}

int main() {
  // In actual game, seed can be random or assigned
  json scenario = scenario_gen();

  // NPC 0 is always the victim
  cout << scenario["npcs"][0]["fullname"].get<string>();
  cout << " was brutally murdered in ";
  cout << scenario["npcs"][0]["pronouns"][2].get<string>();
  cout << " home earlier this evening.\n\n";
  cout << "At the time of the murder...\n";

  for (int i = 1; i < 7; i++) {
    cout << "  " << scenario["npcs"][i]["fullname"].get<string>();
    int alibi = scenario["npcs"][i]["alibi"].get<int>();
    if (alibi == i) {
      cout << " claims to have been alone" ;
    } else {
      cout << " claims to have been with " ;
      cout << scenario["npcs"][alibi]["firstname"].get<string>();
    }
    cout << " in the " << scenario["npcs"][i]["room"].get<string>();
    cout << ".\n";
  }

  cout << "\n";
  cout << scenario["npcs"][6]["firstname"].get<string>();
  cout << " killed ";
  cout << scenario["npcs"][0]["firstname"].get<string>();
  cout << " in the ";
  cout << scenario["npcs"][0]["room"].get<string>();
  cout << " by ";
  cout << scenario["damage"].get<string>();
  cout << " ";
  cout << scenario["npcs"][0]["pronouns"][1].get<string>();
  cout << " with the ";
  cout << scenario["weapon"].get<string>();
  cout << ".\n\n";
  cout << scenario;

  return 0;
}