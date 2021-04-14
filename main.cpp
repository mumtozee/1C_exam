#include <algorithm>
#include <array>
#include <iostream>
#include <stack>
#include <vector>

/* define constants */

const int PILE_COUNT = 8;
const int PILE_SIZE = 9;

/* define classes */
class Pile {
 public:
  Pile() {}
  explicit Pile(std::vector<int> pile, int open_count = 1)
      : pile_(std::move(pile)), open_count_(open_count) {}

  void Push(int card) {
    pile_.push_back(card);
    ++open_count_;
  }

  void Push(const std::vector<int>& temp_pile) {
    for (const auto& card : temp_pile) {
      Push(card);
    }
  }

  void Pop(int num = 1) {
    pile_.erase(pile_.end() - num, pile_.end());
    open_count_ -= num;
    if (open_count_ == 0) {
      open_count_ = 1;
    }
  }

  size_t Size() const { return pile_.size(); }

  int GetOpenTop() const { return pile_[Size() - open_count_]; }

  int GetTop() const { return pile_[Size() - 1]; }

 private:
  std::vector<int> pile_;
  int open_count_;

  friend std::istream& operator>>(std::istream& in, Pile& pile);
  friend class Game;
};

std::istream& operator>>(std::istream& in, Pile& pile) {
  std::vector<int> new_pile_(PILE_SIZE, 0);
  for (int i = 0; i < PILE_SIZE; ++i) {
    std::cin >> new_pile_[i];
  }
  pile.pile_ = std::move(new_pile_);
  pile.open_count_ = 1;
  return in;
}

class Game {
 public:
  bool DefineSolvability() {
    bool answer = false;
    for (int i = 0; i < PILE_COUNT; ++i) {
      bool was_move_in_round = false;
      for (int j = 0; j < PILE_COUNT; ++j) {
        if (j == 0) {
          continue;
        }
        if (piles_[j].GetTop() == piles_[i].GetOpenTop() + 1) {
          std::vector<int> temp_pile(piles_[i].pile_.begin() +
                                         piles_[i].Size() -
                                         piles_[i].open_count_,
                                     piles_[i].pile_.end());
          piles_[j].Push(temp_pile);
          piles_[i].Pop(temp_pile.size());
          was_move_in_round = true;
        }
      }
      answer |= was_move_in_round;
    }
    return answer;
  }

 private:
  std::array<Pile, PILE_COUNT> piles_;
  friend std::istream& operator>>(std::istream& in, Game& game);
};

std::istream& operator>>(std::istream& in, Game& game) {
  for (int i = 0; i < PILE_COUNT; ++i) {
    in >> game.piles_[0];
  }
  return in;
}

int main() {
  Game game{};
  std::cin >> game;
  std::cout << std::boolalpha << game.DefineSolvability() << std::endl;
  return 0;
}
