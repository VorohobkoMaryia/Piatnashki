#ifndef GAME_H
#define GAME_H

#include <QGridLayout>
#include <QWidget>
#include <QMessageBox>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <vector>

class Game : public QWidget {
 Q_OBJECT
 public:
  Game();
  void Initialize();

   public slots:
  void Timeout();
  void Newgame();
  void Retry();
  void Step(int row, int column);
  void resizeEvent(QResizeEvent* event) override;

 public:
  int area = 16;
  QTableWidget box{4, 4, this};
  int empty_cell_index;
  QPushButton new_game{"Новая игра", this};
  QLabel text_seconds{"Уже прошло 0 сек"};
  QPushButton retry{"Повторить игру", this};
  QMessageBox sub_window{};
  std::vector<int> cells{};
  QGridLayout layout{this};
  QLabel text_with_steps{"Количество сделанных ходов равно 0"};
  std::vector<int> start_cells{};
  int num_of_steps = 0;
  int time_in_seconds = 0;
};

#endif //GAME_H
