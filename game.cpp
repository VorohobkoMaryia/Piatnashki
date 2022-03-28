#include "game.h"
#include <QHeaderView>
#include <random>
#include <QTimer>

Game::Game() {
  setWindowTitle("Пятнашки");
  setMinimumSize(730, 390);
  cells = std::vector<int>(area);
  start_cells = std::vector<int>(area);
  Initialize();
  Newgame();
}

void Game::Step(int row, int column) {
  if ((row - empty_cell_index / 4 == 1 || row - empty_cell_index / 4 == -1) && (column - empty_cell_index % 4 == 0) ||
      row - empty_cell_index / 4 == 0 && (column - empty_cell_index % 4 == 1 || column - empty_cell_index % 4 == -1)) {
    box.item(empty_cell_index / 4, empty_cell_index % 4)->setText(
        box.item(row, column)->text());
    std::swap(cells[empty_cell_index], cells[row * 4 + column]);
    box.item(row, column)->setText("");
    empty_cell_index = row * 4 + column;
    text_with_steps.setText(
        "Количество сделанных ходов равно "
        + QString::number(num_of_steps + 1));
    num_of_steps++;
    if (cells.back() != area) {
      return;
    }
    for (int i = 0; i < 15; i++) {
      if (i + 1 != cells[i]) {
        return;
      }
    }
    sub_window.setText(QString("Вы справились за ") +
        QString::number(time_in_seconds) + " сек. и " +
        QString::number(num_of_steps) + " ход(ов/а)!!! Ура!");
    sub_window.open();
  }
}

void Game::Timeout() {
  text_seconds.setText(
      "Уже прошло " + QString::number(time_in_seconds + 1) + " сек");
  time_in_seconds++;
}


void Game::Initialize() {
  box.setEditTriggers(QAbstractItemView::NoEditTriggers);
  box.verticalHeader()->setVisible(false);
  box.horizontalHeader()->setVisible(false);
  layout.addWidget(&text_with_steps, 0, 0, 1, 2, Qt::AlignCenter);
  layout.addWidget(&text_seconds, 1, 0, 1, 2, Qt::AlignCenter);
  layout.addWidget(&new_game, 2, 0);
  layout.addWidget(&retry, 3, 0);
  layout.addWidget(&box, 0, 2, 4, 3);
  sub_window.setWindowTitle("Конец игры");
  box.setFont(QFont("Comic Sans MS", 25));
  for (int counter = 0; counter < area; counter++) {
    box.setItem(counter / 4, counter % 4, new QTableWidgetItem());
    box.item(counter / 4, counter % 4)->setTextAlignment(Qt::AlignCenter);
  }
  retry.setStyleSheet("QPushButton {background-color: purple}");
  new_game.setStyleSheet("QPushButton {background-color: red}");
  box.setStyleSheet(
      "QTableWidget {background-color: rgba(180, 190, 200, 200) }");
  connect(&new_game,
          SIGNAL(clicked(bool)),
          this,
          SLOT(Newgame()));
  connect(&retry, SIGNAL(clicked(bool)), this, SLOT(Retry()));
  auto t = new QTimer(this);
  t->setInterval(1000);
  connect(t, SIGNAL(timeout()), this, SLOT(Timeout()));
  t->start();
  connect(&box, SIGNAL(cellActivated(int,int)),
          this, SLOT(Step(int,int)));
  connect(&sub_window, SIGNAL(finished(int)), this, SLOT(Newgame()));
}

void Game::Newgame() {
  static std::mt19937 generator((std::random_device()()));
  static std::uniform_int_distribution<int> distribution(0, 3);
  for (int i = 0; i < area; i++) {
    cells[i] = i + 1;
  }
  int cur_pos = 15;
  int prev_go = 0;
  std::vector<int> vec = {1,2,-1,-2};
  for (int i = 0; i < 300; i++) {
    int go;
    while (true) {
      do {
        go = vec[distribution(generator)];
      } while (go + prev_go == 0);
      if (go == 1) {
        if (cur_pos % 4 != 0) {
          break;
        }
      } else if (go == 2) {
        if (cur_pos / 4 != 0) {
          break;
        }
      } else if (go == -1) {
        if (cur_pos % 4 != 3) {
          break;
        }
      } else {
        if (cur_pos / 4 != 3) {
          break;
        }
      }
    }
    prev_go = go;
    if (go == 1) {
      std::swap(cells[cur_pos], cells[cur_pos - 1]);
      cur_pos--;
    } else if (go == 2) {
      std::swap(cells[cur_pos], cells[cur_pos - 4]);
      cur_pos -= 4;
    } else if (go == -1) {
      std::swap(cells[cur_pos], cells[cur_pos + 1]);
      cur_pos++;
    } else {
      std::swap(cells[cur_pos], cells[cur_pos + 4]);
      cur_pos += 4;
    }
  }
  start_cells = cells;
  for (int i = 0; i < area; i++) {
    if (start_cells[i] == area) {
      empty_cell_index = i;
      box.item(i / 4, i % 4)->setText("");
    } else {
      box.item(i / 4, i % 4)->setText(QString::number(start_cells[i]));
    }
  }
  text_with_steps.setText("Количество сделанных ходов равно 0");
  num_of_steps = 0;
  text_seconds.setText("Уже прошло 0 сек");
  time_in_seconds = 0;
}

void Game::resizeEvent(QResizeEvent* event) {
  for (int counter = 0; counter < 4; counter++) {
    box.setRowHeight(counter, box.height() / 4 - 1);
    box.setColumnWidth(counter, box.width() / 4 - 1);
  }
}

void Game::Retry() {
  cells = start_cells;
  for (int counter = 0; counter < area; counter++) {
    if (start_cells[counter] != area) {
      box.item(counter / 4,
               counter % 4)->setText(QString::number(start_cells[counter]));
    } else {
      empty_cell_index = counter;
      box.item(counter / 4, counter % 4)->setText("");
    }
  }
  text_with_steps.setText("Количество сделанных ходов равно 0");
  num_of_steps = 0;
  text_seconds.setText("Уже прошло 0 сек");
  time_in_seconds = 0;
}



