#include "MonFix.h"
 #include "data/DataCenter.h"
 #include "data/GIFCenter.h"
 #include "algif5/algif.h"
 #include "shapes/Rectangle.h"
#include<cstdio>

namespace MonFixSetting {
	static constexpr char gif_root_path[50] = "./assets/gif/MonFix";
	static constexpr char gif_postfix[][10] = {
		"appear","stand","attack","die"
	};
}

MonFix* MonFix::create_monfix(int x,int y){
    return new MonFix(x,y,MonFixState::APPEAR);
}

MonFix::MonFix(int x,int y,MonFixState state):state(state){
    for(size_t type=0;type < static_cast<size_t>(MonFixState::MONFIXSTATE_MAX);++type){
        char buffer[50];
	sprintf(
		buffer, "%s/monfix_%s.gif",
		MonFixSetting::gif_root_path,
        MonFixSetting::gif_postfix[static_cast<int>(type)]);
        gifPath[static_cast<MonFixState>(type)]=std::string{buffer};
    }
    //DataCenter*DC=DataCenter::get_instance();
    GIFCenter*GIFC=GIFCenter::get_instance();
    ALGIF_ANIMATION*gif=GIFC->get(gifPath[MonFixState::APPEAR]);
    shape.reset(new Rectangle{
        x-gif->width/2,
        y-gif->height/2,
        x+gif->width/2,
        y+gif->height/2
    });
    //state=MonFixState::APPEAR;
    monfix_erase=false;
}

void MonFix::update(){
    DataCenter* DC = DataCenter::get_instance();
    GIFCenter* GIFC = GIFCenter::get_instance();
    ALGIF_ANIMATION* gif = GIFC->get(gifPath[state]);


    switch (state) {
        case MonFixState::APPEAR:
        //printf("appear now\n");
            if (gif->done) {
                printf("appeargifdone");
                state = MonFixState::STAND;
            }
            break;

        case MonFixState::STAND:
        printf("stand now\n");
            // 在 OperationCenter 中管理狀態變化，因此這裡不需要額外邏輯
            break;

        case MonFixState::ATTACK:
        printf("attack now\n");
            break;

        case MonFixState::DIE:
        //printf("die now\n");
            if (gif->done) {
                monfix_erase = true; // 播放完死亡動畫後標記刪除
            }
            break;

        default:
            break;
    }

    // 檢查當前狀態是否為 APPEAR 且 GIF 撥放已完成
    /*if (state == MonFixState::APPEAR && gif->done) {
        printf("standing");
        state = MonFixState::STAND;  // 切換狀態到 STAND
    }
	if (state == MonFixState::DIE && gif->done) {
            monfix_erase=true;  
    }

    if (state == MonFixState::ATTACK && gif->done) {
        state = MonFixState::STAND;  // 攻擊完成後回到站立狀態
    }*/



}

void MonFix::draw(){
    GIFCenter*GIFC=GIFCenter::get_instance();
    ALGIF_ANIMATION*gif=GIFC->get(gifPath[state]);
    algif_draw_gif(
		gif,
		shape->center_x() - gif->width / 2,
		shape->center_y() - gif->height / 2, 0);
}

int MonFix::get_width(){
    GIFCenter*GIFC=GIFCenter::get_instance();
    ALGIF_ANIMATION*gif=GIFC->get(gifPath[state]);
    return gif->width;
}

int MonFix::get_height(){
    GIFCenter*GIFC=GIFCenter::get_instance();
    ALGIF_ANIMATION*gif=GIFC->get(gifPath[state]);
    return gif->height;
}