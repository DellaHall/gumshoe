#include <iostream>
#include <fstream>
#include <string>
#include <json.hpp>
#include <random>
#include <ctime>

using json = nlohmann::json;

json scenario_gen(int seed = time(nullptr)) {
  auto rng = std::default_random_engine(seed);
  srand(seed);
  json scenario;

  scenario["seed"] = seed;
  // Load dossier
  std::ifstream in_file("resource/dossier.json");
  json dossier;
  in_file >> dossier;

  // Shuffle the alphabet
  std::vector<int> alphabet;
  for (int i = 0; i < 26; i++) {
    alphabet.push_back(i);
  }
  shuffle(alphabet.begin(), alphabet.end(), rng);

  // Generate NPCs
  scenario["npcs"] = {};
  for (int i = 0; i < 7; i++) {
    // Pick NPC Gender
    std::string gender = "X";
    int g = (rand() % 20) + 1;
    if (g >= 1 && g <= 9) { gender = "M"; }
    if (g >= 10 && g <= 18) { gender = "F"; }

    // Pick Pronouns
    std::vector<std::string> pronouns = dossier["pronouns"][gender];

    // Name the NPC
    std::string firstname = dossier[gender][alphabet[i]].get<std::string>();
    std::string lastname = dossier["surnames"][alphabet.end()[-(i + 1)]].get<std::string>();
    // Sometimes make NPCs related
    if (int ((rand()) % 3) == 0) {
      lastname = dossier["surnames"][alphabet.end()[-1]].get<std::string>();
    }

    // Pick NPC's Saluation
    std::vector<std::string> avail_salutes;
    // Always include gender-specific salutations
    avail_salutes.insert(avail_salutes.end(), dossier["salutations"][gender].begin(), dossier["salutations"][gender].end());
    // Sometimes include other salutations
    if (int ((rand()) % 3) == 0) {
      avail_salutes.insert(avail_salutes.end(), dossier["salutations"]["A"].begin(), dossier["salutations"]["A"].end());
    }
    int s = (rand() % avail_salutes.size());
    std::string salutation = avail_salutes[s];

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
  std::vector<std::string> rooms = dossier["rooms"];
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
  std::cout << scenario["npcs"][0]["fullname"].get<std::string>();
  std::cout << " was brutally murdered in ";
  std::cout << scenario["npcs"][0]["pronouns"][2].get<std::string>();
  std::cout << " home earlier this evening.\n\n";
  std::cout << "At the time of the murder...\n";

  for (int i = 1; i < 7; i++) {
    std::cout << "  " << scenario["npcs"][i]["fullname"].get<std::string>();
    int alibi = scenario["npcs"][i]["alibi"].get<int>();
    if (alibi == i) {
      std::cout << " claims to have been alone" ;
    } else {
      std::cout << " claims to have been with " ;
      std::cout << scenario["npcs"][alibi]["firstname"].get<std::string>();
    }
    std::cout << " in the " << scenario["npcs"][i]["room"].get<std::string>();
    std::cout << ".\n";
  }

  std::cout << "\n";
  std::cout << scenario["npcs"][6]["firstname"].get<std::string>();
  std::cout << " killed ";
  std::cout << scenario["npcs"][0]["firstname"].get<std::string>();
  std::cout << " in the ";
  std::cout << scenario["npcs"][0]["room"].get<std::string>();
  std::cout << " by ";
  std::cout << scenario["damage"].get<std::string>();
  std::cout << " ";
  std::cout << scenario["npcs"][0]["pronouns"][1].get<std::string>();
  std::cout << " with the ";
  std::cout << scenario["weapon"].get<std::string>();
  std::cout << ".\n\n";
  std::cout << scenario;

  return 0;
}
