#include "OperationCenter.h"
#include "DataCenter.h"
#include "GIFCenter.h"
#include "../monsters/Monster.h"
#include "../towers/Tower.h"
#include "../towers/Bullet.h"
#include "../exps/Exp.h"
#include "../Player.h"
#include "../Hero.h"
#include "../MonFix.h"
#include "../shapes/Point.h"
#include <allegro5/allegro_primitives.h>
#include <cstdlib>

void OperationCenter::update() {
	// Update monsters.
	_update_monster();
	// Update towers.
	_update_tower();
	// Update tower bullets.
	_update_towerBullet();
	// If any bullet overlaps with any monster, we delete the bullet, reduce the HP of the monster, and delete the monster if necessary.
	_update_monster_towerBullet();
	_update_monfix();
	_update_MonFix_hero();
	// If any monster reaches the end, hurt the player and delete the monster.
	_update_monster_player();
	_update_monster_hero();
	_update_exp_hero();
	//_update_exp();
	//_update_monster_exp();
}

void OperationCenter::_update_monster() {
	std::vector<Monster*> &monsters = DataCenter::get_instance()->monsters;
	for(Monster *monster : monsters)
		monster->update();
}

void OperationCenter::_update_tower() {
	std::vector<Tower*> &towers = DataCenter::get_instance()->towers;
	for(Tower *tower : towers)
		tower->update();
}

void OperationCenter::_update_towerBullet() {
	std::vector<Bullet*> &towerBullets = DataCenter::get_instance()->towerBullets;
	for(Bullet *towerBullet : towerBullets)
		towerBullet->update();
	// Detect if a bullet flies too far (exceeds its fly distance limit), which means the bullet lifecycle has ended.
	for(size_t i = 0; i < towerBullets.size(); ++i) {
		if(towerBullets[i]->get_fly_dist() <= 0) {
			towerBullets.erase(towerBullets.begin()+i);
			--i;
		}
	}
}

void OperationCenter::_update_monfix() {
    DataCenter* DC = DataCenter::get_instance();
    std::vector<MonFix*>& monfixes = DC->monfixes;
    Hero*& hero = DC->hero;
	
    // 增加幀數計數器
    frame_counter++;
	//printf("%d\n",frame_counter);

    // 每300幀生成一個新的 MonFix (300幀約等於5秒鐘，如果遊戲是60 FPS)
    if (frame_counter >= 300) {
        // 創建新 MonFix
        
        //new_monfix->init();

        // 設置新 MonFix 的隨機位置，確保不超出視窗範圍
        //std::mt19937 random_engine(std::random_device{}());
        //std::uniform_int_distribution<int> x_dist(0, DC->window_width - new_monfix->get_width());
        //std::uniform_int_distribution<int> y_dist(0, DC->window_height - new_monfix->get_height());

        //int new_x = x_dist(random_engine);
        //int new_y = y_dist(random_engine);
		srand(time(NULL));
		int new_x=rand()%(DC->window_width+ 1);
		int new_y=rand()%(DC->window_height+ 1);
        MonFix* new_monfix = MonFix::create_monfix(new_x,new_y);

		monfixes.emplace_back(new_monfix);

        // 將新生成的 MonFix 加入列表
        //monfixes.push_back(new_monfix);

        // 重置幀數計數器
        frame_counter = 0;
    }

    // 更新所有 MonFix 的狀態
    for (MonFix* monfix : monfixes) {
        monfix->update();
    }
}

/*void OperationCenter::_update_exp() {
	std::vector<Exp*> &exps = DataCenter::get_instance()->exps;
	for(Exp *exp : exps)
		exp->update();
}*/

void OperationCenter::_update_monster_towerBullet() {
	DataCenter *DC = DataCenter::get_instance();
	std::vector<Monster*> &monsters = DC->monsters;
	std::vector<Bullet*> &towerBullets = DC->towerBullets;
	for(size_t i = 0; i < monsters.size(); ++i) {
		for(size_t j = 0; j < towerBullets.size(); ++j) {
			// Check if the bullet overlaps with the monster.
			if(monsters[i]->shape->overlap(*(towerBullets[j]->shape))) {
				// Reduce the HP of the monster. Delete the bullet.
				monsters[i]->HP -= towerBullets[j]->get_dmg();
				towerBullets.erase(towerBullets.begin()+j);
				--j;
			}
		}
	}
}

void OperationCenter::_update_monster_player() {
	DataCenter *DC = DataCenter::get_instance();
	std::vector<Monster*> &monsters = DC->monsters;
	std::vector<Exp*> &exps = DC->exps;
	Player *&player = DC->player;
	for(size_t i = 0; i < monsters.size(); ++i) {
		// Check if the monster is killed.
		if(monsters[i]->HP <= 0) {
			// Monster gets killed. Player receives money.
			player->coin += monsters[i]->get_money();

			Point death_position(monsters[i]->shape->center_x(), monsters[i]->shape->center_y());
			Exp *new_exp;
			switch(monsters[i]->gettype()){
				case MonsterType::WOLF:
				new_exp = Exp::create_exp(ExpType::LESS, death_position);
				break;
				case MonsterType::CAVEMAN:
				new_exp = Exp::create_exp(ExpType::MEDIUM, death_position);
				break;
			}
            exps.emplace_back(new_exp);

			monsters.erase(monsters.begin()+i);
			--i;
			// Since the current monster is killed, we can directly proceed to next monster.
			break;
		}
		// Check if the monster reaches the end.
		if(monsters[i]->get_path().empty()) {
			monsters.erase(monsters.begin()+i);
			player->HP--;
			--i;
		}
	}
}

void OperationCenter::_update_MonFix_hero(){
	DataCenter *DC = DataCenter::get_instance();
	std::vector<MonFix*> &monfixes = DC->monfixes;
	std::vector<Exp*> &exps = DC->exps;
	Hero *&hero = DC->hero;
	for(size_t i=0;i<monfixes.size();++i){
		monfixes[i]->update();
		Point monfixposition(monfixes[i]->shape->center_x(),monfixes[i]->shape->center_y());
		Point heroposition(hero->shape->center_x(),hero->shape->center_y());
		if(Point::dist(monfixposition,heroposition)<=200){
			monfixes[i]->set_state(MonFixState::ATTACK);
		}
		if(Point::dist(monfixposition,heroposition)>200&&monfixes[i]->get_state()==MonFixState::ATTACK){
			monfixes[i]->set_state(MonFixState::STAND);
		}
		if (monfixes[i]->shape->overlap(*(hero->shape))) {
            monfixes[i]->set_state(MonFixState::DIE);
        }

        // 如果 `monfix_erase` 為真，移除該 `MonFix`
        if (monfixes[i]->monfix_erase) {
			Point death_position(monfixes[i]->shape->center_x(), monfixes[i]->shape->center_y());
			Exp *new_exp = Exp::create_exp(ExpType::LESS, death_position);
            exps.emplace_back(new_exp);
            delete monfixes[i];  // 釋放內存
            monfixes.erase(monfixes.begin() + i);
            --i;  // 調整索引以防跳過下一個元素
        }
	}
}

void OperationCenter::_update_monster_hero(){
	DataCenter *DC = DataCenter::get_instance();
	std::vector<Monster*> &monsters = DC->monsters;
	Hero *&hero = DC->hero;
	for(size_t i=0;i<monsters.size();++i){
		if(monsters[i]->shape->overlap(*(hero->shape))){
			monsters[i]->HP=0;
		}
	}
}

void OperationCenter::_update_exp_hero(){
	DataCenter *DC = DataCenter::get_instance();
	std::vector<Exp*> &exps = DC->exps;
	Hero *&hero = DC->hero;
	//int TotalGain;
	for(size_t i=0;i<exps.size();++i){
		if(exps[i]->shape->overlap(*(hero->shape))){
			DC->TotalGain+=exps[i]->getgain();
			printf("%d\n",DC->TotalGain);
			exps.erase(exps.begin()+i);
			--i;
		}
	}
}

/*void OperationCenter::_update_monster_exp(){
	DataCenter *DC = DataCenter::get_instance();
	std::vector<Monster*> &monsters = DC->monsters;
	std::vector<Exp*> &exps = DC->exps;
	for(size_t i=0;i<monsters.size();++i){
		if(monsters[i]->HP=0){
			create_exp(1,monsters[i]);
		}
	}
}*/

void OperationCenter::draw() {
	_draw_monster();
	_draw_tower();
	_draw_towerBullet();
	_draw_exp();
	_draw_expbar();
	_draw_monfix();
}

void OperationCenter::_draw_monster() {
	std::vector<Monster*> &monsters = DataCenter::get_instance()->monsters;
	for(Monster *monster : monsters)
		monster->draw();
}

void OperationCenter::_draw_tower() {
	std::vector<Tower*> &towers = DataCenter::get_instance()->towers;
	for(Tower *tower : towers)
		tower->draw();
}

void OperationCenter::_draw_towerBullet() {
	std::vector<Bullet*> &towerBullets = DataCenter::get_instance()->towerBullets;
	for(Bullet *towerBullet : towerBullets)
		towerBullet->draw();
}

void OperationCenter::_draw_exp(){
	std::vector<Exp*> &exps = DataCenter::get_instance()->exps;
	for(Exp *exp : exps)
		exp->draw();
}

void OperationCenter::_draw_expbar(){
	DataCenter *DC = DataCenter::get_instance();
	//printf("test\n");
	al_draw_filled_rectangle(20, 20,20+DC->TotalGain, 40, al_map_rgb(255,0,0));
}

void OperationCenter::_draw_monfix(){
	std::vector<MonFix*> &monfixes = DataCenter::get_instance()->monfixes;
	for(MonFix *monfix : monfixes)
		monfix->draw();
}