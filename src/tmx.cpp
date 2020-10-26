#include <iostream>
#include <fstream>
#include <string>

#include <SFML/Graphics.hpp>
#include <rapidxml.hpp>


using namespace std;

class Tmx {
public:
  bool load(const string& tmxFile) {
    rapidxml::xml_document<> doc;

    ifstream in_file(tmxFile);
    if (!in_file) return false;

    vector<char> buffer((istreambuf_iterator<char>(in_file)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    doc.parse<0>(&buffer[0]);

    // I think what i need to do is for thru the xml, and convert it to a json...

    rapidxml::xml_node<> *root = doc.first_node("map");
    rapidxml::xml_node<> *child = root->first_node("tileset");

    cout << child;

    return true;
  }

};

class TileMap : public sf::Drawable, public sf::Transformable {
public:

  bool load(const string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
  {
    // load the tileset texture
    if (!m_tileset.loadFromFile(tileset))
        return false;

    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width * height * 4);

    // populate the vertex array, with one quad per tile
    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j)
        {
            // get the current tile number
            int tileNumber = tiles[i + j * width];

            // find its position in the tileset texture
            int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
            int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

            // get a pointer to the current tile's quad
            sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

            // define its 4 corners
            quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

            // define its 4 texture coordinates
            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
        }
      return true;
  }

private:

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
  {
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &m_tileset;

    // draw the vertex array
    target.draw(m_vertices, states);
  }

  sf::VertexArray m_vertices;
  sf::Texture m_tileset;
};

int main() {
  Tmx map1;
  if (!map1.load("resource/maps/map1.tmx")) return -1;

  return 0;
}
