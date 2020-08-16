#include <iostream>
#include <fstream>
#include <string>
#include <json.hpp>
#include <random>
#include <ctime>
#include "scenario.h"

/** Generate a sleuth-style scenario */
// Pick 7 characters from dossier.json
// Assign each a surname and appropriate pronouns & salutations
// Pick a victim
// Pick a murderer
// Create alibis



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
  json npcs;
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

    cout << salutation << " ";
    cout << firstname << " ";
    cout << lastname << " ";
    cout << gender << " ";
    cout << pronouns[0] << " ";
    cout << pronouns[1] << " ";
    cout << pronouns[2] << " ";
    cout << pronouns[3] << " ";
    cout << "\n";
  }

  // NPC 0 is always the victim

  return 0;
}