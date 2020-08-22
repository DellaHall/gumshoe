#include <iostream>
#include <fstream>
#include <string>
#include <json.hpp>
#include <random>
#include <ctime>

using namespace std;
using json = nlohmann::json;

int main();


int main() {
  // In actual game, seed can be random or assigned
  unsigned int seed = time(nullptr);
  auto rng = std::default_random_engine(seed);
  srand(seed);
  json scenario;

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

  // NPC 0 is always the victim
  cout << scenario["npcs"][0]["fullname"].get<string>();
  cout << " was brutally murdered in ";
  cout << scenario["npcs"][0]["pronouns"][2].get<string>();
  cout << " home earlier this evening.\n\n";
  cout << "At the time of the murder...\n";
  
  // NPC 1
  cout << "  " << scenario["npcs"][1]["fullname"].get<string>();
  cout << " claims to have been with " << scenario["npcs"][2]["firstname"].get<string>() << ".\n";
  // NPC 2
  cout << "  " << scenario["npcs"][2]["fullname"].get<string>();
  cout << " claims to have been with " << scenario["npcs"][1]["firstname"].get<string>() << ".\n";
  // NPC 3
  cout << "  " << scenario["npcs"][3]["fullname"].get<string>();
  cout << " claims to have been with " << scenario["npcs"][4]["firstname"].get<string>() << ".\n";
  // NPC 4
  cout << "  " << scenario["npcs"][4]["fullname"].get<string>();
  cout << " claims to have been with " << scenario["npcs"][3]["firstname"].get<string>() << ".\n";
  // NPC 5
  cout << "  " << scenario["npcs"][5]["fullname"].get<string>();
  cout << " claims to have been alone.\n";
  // NPC 6 - The Murderer
  cout << "  " << scenario["npcs"][6]["fullname"].get<string>();
  cout << " claims to have been with " << scenario["npcs"][4]["firstname"].get<string>() << ".\n";

  return 0;
}