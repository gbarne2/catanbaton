#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gameClient.h"
#include <QPushButton>

#define Acorn 0
#define Bcorn 1
#define Ccorn 2
#define Dcorn 3
#define Ecorn 4
#define Fcorn 5

#define cornA	0
#define cornB	1
#define cornC	2
#define cornD	3
#define cornE	4
#define cornF	5

#define setwval 1

namespace Ui {
class MainWindow;
}

extern int devcardflag;
extern int dv_build_roads_flag;
extern int dv_play_knight_flag;
extern int dv_yop_flag;
extern int dv_monopoly_flag;
extern int dv_victory_point_flag;
extern int game_started;
extern int request_user_place_robber;

class MainWindow : public QMainWindow
{
    QTimer *timer;
    Q_OBJECT
    int* buttonstatus;
    int server_connected;
    int devcardtouse;
    int place_init_settlement;
    void update_resources_display();
    string print_board();
    int updatecolortile;
    int updateboardcolors;
    int droll(int);
    int already_printed_board;
    int init_set_ready;
    int init_placement_corner;
    int init_set_placement_tile;
    int init_road_placement_road;
    int init_road_placement_tile;
    int corner_info(int corner, int tilenum);
    void setDiceRoll(std::string nname, QPushButton *ptr, int x);
    void update_dev_cards_on_gui();
    void prompt_dev_card_to_use();
    void use_dev_card_generic(const QString);
    void check_packet_and_update(int);
    int Check_and_process_flags();

private slots:
    int Init_game_and_connection();

public:
    void setdicerolls();
    void set_button_color(std::string nname, QPushButton *ptr, int x);
    explicit MainWindow(QWidget *parent = 0);
    int update_board_colors();
    void set_icons_and_rollvals_on_board();
    void get_icon_file_rsrc_type_and_roll_from_tile_num(QString &filename, int &resourcetype, int &roll, int tile);
    ~MainWindow();
public slots:
    void on_pushButton_clicked();
    void on_pushButton_released();
    void on_hover();
    void check_rx_packet();

public: signals:
    void hovered();

private:
    Ui::MainWindow *ui;
    gameClient Cgame;
};

extern char rxdatabuff [4096];

#endif // MAINWINDOW_H
