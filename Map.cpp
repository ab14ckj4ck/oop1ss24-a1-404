#include "Map.hpp"

Map::Map(char* config_path, Player* player_a, Player* player_b) : output_active_(true) {
  loadConfig(config_path, player_a, player_b);
}

Map::~Map() {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < columns_; j++) {
      delete fields_[i][j];
    }
  }
}

void Map::loadConfig(char* config_path, Player* player_a, Player* player_b) {
  std::ifstream file(config_path);
  std::string line;
  std::getline(file, line);
  std::getline(file, line, '_');
  columns_ = std::stoi(line);
  std::getline(file, line);
  rows_ = std::stoi(line);

  fields_.resize(rows_, std::vector<Field*>(columns_)); // vergrößert den Vektor auf rows x columns

  int row_cntr = 0;
  int col_cntr = 0;
  while (std::getline(file, line)) {
    col_cntr = 0;
    for (char c : line) {
      switch (c) {
        case 'a': // player A
          fields_[row_cntr][col_cntr++] = new Field(player_a, 1, false);
          player_a->addClaimedFields(1);
          continue;

        case 'b': // player B
          fields_[row_cntr][col_cntr++] = new Field(player_b, 1, false);
          player_b->addClaimedFields(1);
          continue;

        case '-': // water field
          fields_[row_cntr][col_cntr++] = new Field(nullptr, 0, true);
          continue;

        case '0': // empty field
          fields_[row_cntr][col_cntr++] = new Field(nullptr, 0, false);
          continue;
      }
    }
    row_cntr++;
  }
}

void Map::printMap() {
  if (output_active_) {
    // top line
    std::cout << "  ";
    for (int i = 0; i < columns_; i++) {
      std::cout << "| " << i + 1 << " ";
    }
    std::cout << std::endl;

    // fields
    for (int i = 0; i < rows_; i++) {
      std::cout << i + 1 << " ";
      for (int j = 0; j < columns_; j++) {
        fields_[i][j]->printField();
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
  else {
    return;
  }
}

void Map::setOutputActive() {
  if (output_active_) {
    output_active_ = false;
  }
  else {
    output_active_ = true;
    printMap();
  }
}

Field* Map::getField(int x, int y) {
  return fields_[y][x];
}

int Map::getColumns() {
  return columns_;
}

int Map::getRows() {
  return rows_;
}
