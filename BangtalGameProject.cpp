#define NUM_OF_LABORATORY_SCENES 19
#define ELEVATOR_TIME 4.0f
#define SCENE_FRAME_RATE 0.4f


#define _CRT_SECURE_NO_WARNINGS


#include <bangtal.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>

SceneID main_scene;

SceneID laboratory_scenes[NUM_OF_LABORATORY_SCENES];
SceneID elevator_scene, elevator_plate_scene;
SceneID screen_scene;

SceneID cur_scene;

//start button
ObjectID startButton;

//dir buttons, elevator switch
ObjectID dir_left[NUM_OF_LABORATORY_SCENES], dir_right[NUM_OF_LABORATORY_SCENES], dir_up[NUM_OF_LABORATORY_SCENES], dir_down[NUM_OF_LABORATORY_SCENES];
ObjectID dir_elevator_up;
ObjectID dir_elevator_plate_down;
ObjectID dir_sceen_down;


/*
공통
*/
//windows
ObjectID laboratory_box_window_close, laboratory_box_window_open;
ObjectID laboratory_chamber_window_close, laboratory_chamber_window_open;

// 기타 객체
ObjectID laboratory_plant_pot;

// 금고
ObjectID laboratory_keypad_1f,laboratory_keypad_2f, laboratory_keypad_3f;
ObjectID laboratory_safe_1f, laboratory_safe_2f, laboratory_safe_3f;
bool lock_opend[3] = { false, };

// 시계 금고
ObjectID laboratory_clock_1f, laboratory_clock_2f, laboratory_clock_3f;
ObjectID laboratory_clock_safe_1f, laboratory_clock_safe_2f, laboratory_clock_safe_3f;

// 화면
ObjectID laboratory_screen;


// 탈출구
ObjectID laboratory_3f_endingDoor;

//조합결과물
ObjectID vaccine;

/*
층별
*/

//elevator plate
ObjectID elevator_1f_button, elevator_2f_button, elevator_3f_button;
ObjectID elevator_plate;
ObjectID elevator_screw[2][2];
ObjectID elevator_circuit;
ObjectID elevator_battery, elevator_cable, elevator_battery_full;

//video

ObjectID video_zombie;

//1층 객체
ObjectID laboratory_1f_gmtPaper;
ObjectID laboratory_1f_water;
ObjectID laboratory_1f_seed;
ObjectID laboratory_1f_driver;
ObjectID laboratory_1f_connector;



//2층 객체
ObjectID laboratory_2f_flask;
ObjectID laboratory_2f_cardTagger_16, laboratory_2f_cardTagger_15;
ObjectID laboratory_2f_greenCard;
ObjectID laboratory_2f_blackCard;
ObjectID laboratory_2f_cardTagger_chamber;
ObjectID laboratory_2f_stem;
ObjectID laboratory_2f_vaccine_recipe;
ObjectID laboratory_2f_finalKey;
ObjectID laboratory_2f_charger;
ObjectID laboratory_2f_wire;
ObjectID laboratory_2f_plug;


//3층 객체
ObjectID laboratory_3f_passwordPaper;
ObjectID laboratory_3f_letter;
ObjectID laboratory_3f_cardTagger_16, laboratory_3f_cardTagger_15;
ObjectID laboratory_3f_blackCard;
ObjectID laboratory_3f_stem, laboratory_3f_flower;
ObjectID laboratory_3f_zombie_17;


TimerID elevator_move_timer, elevator_open_timer, scene_change_timer, ending_timer;

SoundID elevator_move_sound, elevator_arrive_sound, elevator_button_sound, elevator_blackout_sound, zombie_sound, confusion_sound;
SoundID auto_window_close_sound, auto_window_open_sound, manual_window_close_sound, manual_window_open_sound;
SoundID laboratory_1f_bgm, laboratory_2f_bgm,laboratory_3f_bgm;
SoundID soil_sound, water_sound, generator_sound, door_sound, unlock_sound, breakDown_sound, beep_sound, electro_sound;
SoundID current_bgm;

bool elevator_moving = false;
bool door15_closed[3] = {false, false, true};
bool stage_1_clear = false, stage_2_clear = false, stage_3_clear= false;
bool zombie_appear = false;
bool laboratory_2f_chamber_close = true;
bool laboratory_1f_seedOnPot=false, laboratory_1f_waterOnPot=false;
bool vaccineMade =false;
bool elevator_plate_closed = true;
bool screw_removed[2][2] = { false, };
bool battery_empty = false;
bool charger_connected = false, concent_plugged=false;


int current_floor = -1, target_floor = -1;

int scene_frame = 0; //for scene change

/*
-------------------------------------------------------------------------------------
|																					|
|								Run time process									|
|																					|
|																					|
-------------------------------------------------------------------------------------
*/
void timerCallback(TimerID timer);
void mouseCallback(ObjectID object, int x, int y, MouseAction action);
void objectCallback(ObjectID object, EventID e);

void enterSceneChangeCur(SceneID scene);
void hideLaboratoryObject(int floor);
void setLaboratoryScene(int floor);
void elevatorMove(int goal);
void hideElevatorObject();
void showElevatorObject();
void showEscapeVideo();

/*
-------------------------------------------------------------------------------------
|																					|
|								static process										|
|																					|
|																					|
-------------------------------------------------------------------------------------
*/
void scene_init();
void dir_init();
void up_init(int i, int xpos = 630,int ypos=20);
void down_init(int i, int xpos = 630,int ypos=20);
void object_init();//TODO: 각 scene별로 객체 위치 시키기
void timer_init();//타이머 생성 및 초기화
void sound_init();//음원 생성 및 초기화

int main()
{
	setMouseCallback(mouseCallback);
	setObjectCallback(objectCallback);
	setTimerCallback(timerCallback);
	
	scene_init();
	dir_init();
	object_init();
	timer_init();
	sound_init();

	defineCombination(laboratory_3f_flower, laboratory_2f_flask, vaccine);
	cur_scene =main_scene;
	startGame(cur_scene);
}

/*
-------------------------------------------------------------------------------------
|																					|
|								Run time process									|
|																					|
|																					|
-------------------------------------------------------------------------------------
*/

void timerCallback(TimerID timer)
{
	if (timer == scene_change_timer)
	{
		if (scene_frame == 0)
		{
			setSceneImage(elevator_scene, "images/scenes/future17.png");
			scaleObject(video_zombie, 1.4f);
			locateObject(video_zombie,elevator_scene,450,30);
			setTimer(scene_change_timer, SCENE_FRAME_RATE);
			startTimer(scene_change_timer);
			
			scene_frame++;
		}
		else if (scene_frame == 1)
		{
			hideObject(video_zombie);
			setSceneImage(elevator_scene, "images/scenes/future14.png");
			setTimer(scene_change_timer, SCENE_FRAME_RATE);
			startTimer(scene_change_timer);
			scene_frame++;
		}
		else if (scene_frame == 2)
		{
			setSceneImage(elevator_scene, "images/scenes/future12.png");
			setTimer(scene_change_timer, SCENE_FRAME_RATE);
			startTimer(scene_change_timer);
			scene_frame++;
		}
		else if (scene_frame == 3) {
			locateObject(video_zombie, elevator_scene, 950, 30);
			scaleObject(video_zombie,1.2f);
			showObject(video_zombie);

			setSceneImage(elevator_scene, "images/scenes/future11.png");
			setTimer(scene_change_timer, SCENE_FRAME_RATE);
			startTimer(scene_change_timer);
			scene_frame++;
		}
		else if (scene_frame==4)
		{
			locateObject(video_zombie, elevator_scene, 500, 30);
			scaleObject(video_zombie, 1.4f);

			showElevatorObject();
			setSceneImage(elevator_scene, "images/scenes/elevator-open-future.png");
			setTimer(scene_change_timer, SCENE_FRAME_RATE);
			startTimer(scene_change_timer);
			scene_frame++;
		}
		else if (scene_frame == 5)
		{
			hideObject(video_zombie);
			stopSound(zombie_sound);
			stopSound(current_bgm);
			setSceneImage(elevator_scene, "images/scenes/elevator-closed.png");
			setTimer(scene_change_timer, SCENE_FRAME_RATE);
			startTimer(scene_change_timer);
			scene_frame++;
		}
		else if (scene_frame == 6 && current_floor == 3)
		{
			showMessage("방금 뭐였지?");
		}
		else if (scene_frame == 6 && current_floor == -1)
		{
			hideElevatorObject();
			playSound(confusion_sound);
			setSceneImage(elevator_scene, "images/scenes/dark.png");
			setTimer(scene_change_timer, 1.0f);
			startTimer(scene_change_timer);
			scene_frame++;
		}
		else if (scene_frame == 7 && current_floor == -1)
		{
			showMessage("???:... 실패");
			setTimer(scene_change_timer, 5.0f);
			startTimer(scene_change_timer);
			scene_frame++;
		}
		else if (scene_frame == 8 && current_floor == -1)
		{
			setSceneImage(elevator_scene, "images/scenes/elevator-closed.png");
			showElevatorObject();
			showMessage("여기가 어디지?");
		}
	}
	if (timer== elevator_move_timer)
	{
		stopSound(elevator_move_sound);
		if (target_floor == 1 && current_floor==-1)
		{
			playSound(elevator_blackout_sound);
			setSceneLight(elevator_scene, 0.1f);
			setTimer(elevator_open_timer, 3.0f);
			startTimer(elevator_open_timer);
		}
		else if (target_floor == 1 && current_floor != -1)
		{
			playSound(elevator_arrive_sound);
			setSceneImage(elevator_scene, "Images/scenes/elevator-open-past.png");
			showObject(dir_elevator_up);
			elevator_moving = false;
			hideLaboratoryObject(current_floor);
			setLaboratoryScene(target_floor);
			playSound(laboratory_1f_bgm, true);
			current_bgm = laboratory_1f_bgm;
		}
		else if (target_floor ==2 && stage_2_clear)
		{
			playSound(elevator_arrive_sound);
			setSceneImage(elevator_scene, "Images/scenes/elevator-open-present.png");
			showObject(dir_elevator_up);
			elevator_moving = false;
			hideLaboratoryObject(current_floor);
			setLaboratoryScene(target_floor);
			playSound(laboratory_2f_bgm, true);
			current_bgm = laboratory_2f_bgm;
		}
		else if (target_floor == 2 && !stage_2_clear )
		{
			playSound(elevator_arrive_sound);
			setSceneLight(elevator_scene, 0.1f);
			playSound(breakDown_sound);
			setTimer(elevator_open_timer, 3.0f);
			startTimer(elevator_open_timer);

		}
		else if (target_floor == 3 )
		{
			playSound(elevator_arrive_sound);
			setSceneImage(elevator_scene, "Images/scenes/elevator-open-future.png");
			showObject(dir_elevator_up);
			elevator_moving = false;
			hideLaboratoryObject(current_floor);
			setLaboratoryScene(target_floor);
			playSound(laboratory_3f_bgm, true);
			current_bgm = laboratory_3f_bgm;
		}
	}
	else if (timer == elevator_open_timer && current_floor==-1)
	{
		setSceneImage(elevator_scene, "Images/scenes/elevator-open-past.png");
		setSceneLight(elevator_scene, 0.5f);
		setSceneLight(elevator_plate_scene, 0.5f);
		showObject(dir_elevator_up);
		elevator_moving = false;
		setLaboratoryScene(1);
		playSound(laboratory_1f_bgm,true);
		current_bgm = laboratory_1f_bgm;
	}
	else if (timer == elevator_open_timer && stage_1_clear && !stage_2_clear)
	{
		setSceneImage(elevator_scene, "Images/scenes/elevator-open-present.png");
		setSceneLight(elevator_scene, 0.5f);
		setSceneLight(elevator_plate_scene, 0.5f);
		showObject(dir_elevator_up);
		elevator_moving = false;
		hideLaboratoryObject(1);
		setLaboratoryScene(2);
		playSound(laboratory_2f_bgm, true);
		current_bgm = laboratory_2f_bgm;
		setObjectImage(elevator_battery, "Images/empty-battery.png");
		battery_empty = true;
		showMessage("배터리가 방전된 것 같다.");
	}
	else if (timer == ending_timer && scene_frame==0)
	{
		scene_frame++;
		showMessage("???: 102번째 테스트 성공");
		setTimer(ending_timer, 4.0f);
		startTimer(ending_timer);
	}
	else if (timer == ending_timer && scene_frame == 1)
	{
		scene_frame++;
		showMessage("???: 103번째 테스트 준비");
		setTimer(ending_timer, 4.0f);
		startTimer(ending_timer);
	}
	else if (timer == ending_timer && scene_frame == 2)
	{
		scene_frame++;
		playSound(confusion_sound);
		setSceneImage(main_scene, "Images/scenes/elevator-closed.png");
		setTimer(ending_timer, 4.0f);
		startTimer(ending_timer);
	}
	else if (timer == ending_timer && scene_frame == 3)
	{
		scene_frame++;
		showMessage("플레이 해주셔서 감사합니다.");
		setTimer(ending_timer, 4.0f);
		startTimer(ending_timer);
	}
	else if (timer == ending_timer && scene_frame == 4)
	{
		endGame();
	}
}
void mouseCallback(ObjectID object, int x, int y, MouseAction action)
{
	int cur = -1;
	if (cur_scene == main_scene) {
		if (object == startButton)
		{
			playSound(zombie_sound);
			enterSceneChangeCur(elevator_scene);
			showObject(video_zombie);
			showEscapeVideo();
		}
	}
	else if (cur_scene == laboratory_scenes[0]) {
		cur = 0;
		if (object == dir_left[cur])
			enterSceneChangeCur(laboratory_scenes[13]);
		else if (object == dir_right[cur])
			enterSceneChangeCur(laboratory_scenes[1]);
		else if (object == dir_down[cur])
		{
			if (door15_closed[current_floor - 1])
				enterSceneChangeCur(laboratory_scenes[15]);
			else
				enterSceneChangeCur(laboratory_scenes[16]);
		}
	}
	else if (cur_scene == laboratory_scenes[1]) {
		cur = 1;
		if (object == dir_left[cur])
			enterSceneChangeCur(laboratory_scenes[0]);
		else if (object == dir_right[cur])
			enterSceneChangeCur(laboratory_scenes[2]);
		else if (object == laboratory_keypad_1f)
		{
			showKeypad("4281", laboratory_keypad_1f);
		}
		else if (object == laboratory_keypad_2f)
		{
			showKeypad("7139", laboratory_keypad_2f);
		}
		else if (object == laboratory_keypad_3f)
		{
			showKeypad("2522", laboratory_keypad_3f);
		}
	}
	else if (cur_scene == laboratory_scenes[2]) {
		cur = 2;
		if (object == dir_left[cur])
			enterSceneChangeCur(laboratory_scenes[1]);
		else if (object == dir_right[cur])
			enterSceneChangeCur(laboratory_scenes[3]);
		else if (object == dir_down[cur])
			enterSceneChangeCur(laboratory_scenes[18]);
		else if (getHandObject() == laboratory_1f_seed && laboratory_plant_pot && current_floor == 1)
		{
			dropObject(laboratory_1f_seed);
			hideObject(laboratory_1f_seed);
			playSound(soil_sound);
			laboratory_1f_seedOnPot = true;
		}
		else if(getHandObject() == laboratory_1f_water && laboratory_plant_pot && current_floor == 1 && laboratory_1f_seedOnPot==true)
		{
			dropObject(laboratory_1f_water);
			hideObject(laboratory_1f_water);
			playSound(water_sound);

			laboratory_1f_waterOnPot = true;
		}
	}
	else if (cur_scene == laboratory_scenes[3]) {
		cur = 3;
		if (object == dir_left[cur])
			enterSceneChangeCur(laboratory_scenes[2]);
		else if (object == dir_right[cur])
			enterSceneChangeCur(laboratory_scenes[5]);
		else if (object == dir_up[cur])
			enterSceneChangeCur(laboratory_scenes[4]);
	}
	else if (cur_scene == laboratory_scenes[4]) {
		cur = 4;
		if (object == dir_left[cur])
			enterSceneChangeCur(laboratory_scenes[13]);
		else if (object == dir_right[cur])
			enterSceneChangeCur(laboratory_scenes[3]);
	}
	else if (cur_scene == laboratory_scenes[5]) {
		cur = 5;
		if (object == dir_left[cur])
			enterSceneChangeCur(laboratory_scenes[3]);
		else if (object == dir_right[cur])
			enterSceneChangeCur(laboratory_scenes[8]);
		else if (object == dir_up[cur]) 
			enterSceneChangeCur(laboratory_scenes[7]);
	}
	else if (cur_scene == laboratory_scenes[6]) {
		cur = 6;
		if (object == dir_left[cur])
			enterSceneChangeCur(laboratory_scenes[7]);
		else if (object == dir_right[cur])
			enterSceneChangeCur(laboratory_scenes[5]);
	}
	else if (cur_scene == laboratory_scenes[7]) {
		cur = 7;
		if (object == dir_left[cur])
			enterSceneChangeCur(laboratory_scenes[11]);
		else if (object == dir_right[cur])
			enterSceneChangeCur(laboratory_scenes[5]);
		else if (object == laboratory_2f_cardTagger_chamber && getHandObject() == laboratory_2f_greenCard && laboratory_2f_chamber_close == false)
		{
			playSound(auto_window_close_sound);
			hideObject(laboratory_chamber_window_open);
			showObject(laboratory_chamber_window_close);
			laboratory_2f_chamber_close = true;
		}
		else if (object == laboratory_2f_cardTagger_chamber && getHandObject() == laboratory_2f_greenCard && laboratory_2f_chamber_close == true)
		{
			playSound(auto_window_open_sound);
			hideObject(laboratory_chamber_window_close);
			showObject(laboratory_chamber_window_open);
			laboratory_2f_chamber_close = false;
		}
	}
	else if (cur_scene == laboratory_scenes[8]) {
		cur = 8;

		if (object == dir_left[cur])
			enterSceneChangeCur(laboratory_scenes[5]);
		else if (object == dir_right[cur])
			enterSceneChangeCur(laboratory_scenes[9]);
		else if (object == laboratory_clock_1f)
		{
			showKeypad("2339", laboratory_clock_1f);
		}
		else if (object == laboratory_clock_2f)
		{
			showKeypad("0015", laboratory_clock_2f);
		}
		else if (object == laboratory_clock_3f)
		{
			showKeypad("0318", laboratory_clock_3f);
		}
	}
	else if (cur_scene == laboratory_scenes[9]) {
		cur = 9;

		if (object == dir_left[cur])
			enterSceneChangeCur(laboratory_scenes[8]);
		else if (object == dir_right[cur])
			enterSceneChangeCur(laboratory_scenes[10]);
		else if (object == laboratory_screen)
		{
			if (current_floor == 1)
			{
				setSceneImage(screen_scene, "Images/scenes/past-page.png");
			}
			else if (current_floor == 2)
			{
				setSceneImage(screen_scene, "Images/scenes/present-page.png");
			}
			else if (current_floor == 3)
			{
				setSceneImage(screen_scene, "Images/scenes/future-page.png");
			}
			enterSceneChangeCur(screen_scene);
		}

	}
	else if (cur_scene == laboratory_scenes[10]) {
		cur = 10;
		if (object == dir_left[cur])
			enterSceneChangeCur(laboratory_scenes[9]);
		else if (object == dir_right[cur])
			enterSceneChangeCur(laboratory_scenes[11]);
		else if (object == dir_down[cur])
			enterSceneChangeCur(elevator_scene);
	}
	else if (cur_scene == laboratory_scenes[11]) {
		cur = 11;
		if (object == dir_left[cur])
			enterSceneChangeCur(laboratory_scenes[10]);
		else if (object == dir_right[cur])
			enterSceneChangeCur(laboratory_scenes[13]);
		else if (object == dir_up[cur])
			enterSceneChangeCur(laboratory_scenes[7]);
	}
	else if (cur_scene == laboratory_scenes[12]) {
		cur = 12;
		if (object == dir_left[cur])
			enterSceneChangeCur(laboratory_scenes[11]);
		else if (object == dir_right[cur])
			enterSceneChangeCur(laboratory_scenes[7]);
	}
	else if (cur_scene == laboratory_scenes[13]) {
		cur = 13;
		if (object == dir_left[cur])
			enterSceneChangeCur(laboratory_scenes[11]);
		else if (object == dir_right[cur])
			enterSceneChangeCur(laboratory_scenes[0]);
		else if (object == dir_up[cur])
			enterSceneChangeCur(laboratory_scenes[4]);
	}
	else if (cur_scene == laboratory_scenes[14]) {
		cur = 14;
		if (object == dir_left[cur])
			enterSceneChangeCur(laboratory_scenes[13]);
		else if (object == dir_right[cur])
			enterSceneChangeCur(laboratory_scenes[4]);
	}
	else if (cur_scene == laboratory_scenes[15]) {
		cur = 15;
		if (object == dir_left[cur])
			enterSceneChangeCur(laboratory_scenes[1]);
		else if (object == dir_right[cur])
			enterSceneChangeCur(laboratory_scenes[13]);
		else if (object == dir_down[cur])
			enterSceneChangeCur(laboratory_scenes[0]);
		else if (object == laboratory_2f_cardTagger_15 && (getHandObject() == laboratory_2f_blackCard))
		{
			door15_closed[1] = false;
			playSound(beep_sound);
			door15_closed[2] = false;

			enterSceneChangeCur(laboratory_scenes[16]);
		}
		else if (object == laboratory_3f_cardTagger_15 && getHandObject() == laboratory_3f_blackCard)
		{
			playSound(beep_sound);
			door15_closed[2] = false;
			enterSceneChangeCur(laboratory_scenes[16]);
		}

	}
	else if (cur_scene == laboratory_scenes[16]) {
		cur = 16;
		if (object == dir_left[cur])
			enterSceneChangeCur(laboratory_scenes[1]);
		else if (object == dir_right[cur])
			enterSceneChangeCur(laboratory_scenes[13]);
		else if (object == dir_down[cur])
			enterSceneChangeCur(laboratory_scenes[0]);
		else if (object == dir_up[cur])
		{
			if (current_floor == 3)
			{
				// 도주 장면 //chap 3 종료
				playSound(zombie_sound);
				hideElevatorObject();
				enterSceneChangeCur(elevator_scene);
				showObject(video_zombie);
				showEscapeVideo();
				hideObject(dir_elevator_up);
				zombie_appear = true;
				dropObject(laboratory_3f_blackCard);
				hideObject(laboratory_3f_blackCard);
			}
			else
				enterSceneChangeCur(laboratory_scenes[17]);

		}
		else if (object == laboratory_2f_cardTagger_16 && getHandObject() == laboratory_2f_blackCard)
		{
			door15_closed[1] = true;
			door15_closed[2] = true;
			playSound(beep_sound);
			enterSceneChangeCur(laboratory_scenes[15]);
		}
		else if (object == laboratory_3f_cardTagger_16 && getHandObject() == laboratory_3f_blackCard)
		{
			door15_closed[2] = true;
			playSound(beep_sound);
			enterSceneChangeCur(laboratory_scenes[15]);
		}
	}
	else if (cur_scene == laboratory_scenes[17]) {
		cur = 17;
		if (object == dir_down[cur])
			enterSceneChangeCur(laboratory_scenes[16]);
		else if (object == laboratory_2f_charger && getHandObject() == elevator_battery && charger_connected == true)
		{
			showMessage("충전이 완료됐다.");
			stopSound(generator_sound);
			setObjectImage(elevator_battery, "Images/full-battery.png");
			battery_empty = false;
		}
		else if (object == laboratory_2f_wire && concent_plugged==false && getHandObject()==laboratory_2f_plug) 
		{
			dropObject(laboratory_2f_plug);
			hideObject(laboratory_2f_plug);
			setObjectImage(laboratory_2f_wire, "Images/socket-disconnect.png");
			concent_plugged = true;
		}
		else if (object == laboratory_2f_wire && concent_plugged == true && charger_connected == false && getHandObject()==laboratory_1f_driver)
		{
			setObjectImage(laboratory_2f_wire, "Images/socket-plugged.png");
			charger_connected = true;
			playSound(generator_sound);
		}
	}
	else if (cur_scene == laboratory_scenes[18]) {
		cur = 18;
		if (object == dir_left[cur])
			enterSceneChangeCur(laboratory_scenes[3]);
		else if (object == dir_right[cur])
			enterSceneChangeCur(laboratory_scenes[1]);
		else if (object == dir_down[cur])
			enterSceneChangeCur(laboratory_scenes[2]);
		else if (getHandObject() == laboratory_2f_finalKey && object == laboratory_3f_endingDoor)
		{
			if (vaccineMade == false)
			{
				showMessage("이대로 나가는 건 위험한것 같다. 백신은 완성하고 탈출해야 겠다.");
			}
			else {
				
				playSound(door_sound);
				stopSound(current_bgm);
				hideObject(startButton);
				enterSceneChangeCur(main_scene);
				scene_frame = 0;
				setTimer(ending_timer, 5.0f);
				startTimer(ending_timer);
			}
		}
		else if ( !(getHandObject() == laboratory_2f_finalKey) && object == laboratory_3f_endingDoor)
		{
			showMessage("문이 잠겨 있다.");
		}
	}
	else if (cur_scene == elevator_scene) {
		cur = 18;
		if (object == dir_elevator_up)
			enterSceneChangeCur(laboratory_scenes[10]);
		else if (object == elevator_1f_button)
		{
			if (elevator_moving == true)
			{
				//아무런 동작안함
			}
			else if (current_floor == 1 && stage_1_clear == false)
			{
				showMessage("고장난것 같다. 우선 수리를 해야할것 같다.");
			}
			else if (current_floor == 1 && stage_1_clear == true && stage_2_clear == false)
			{
				//아무런 동작 안함
			}
			else if (current_floor == 2 && stage_1_clear == true && stage_2_clear == false)
			{
				showMessage("배터리가 나간것 같다. 배터리를 충전해야 할것 같다.");
			}
			else if (current_floor != 1)
			{
				playSound(elevator_button_sound);
				elevatorMove(1);
			}
		}
		else if (object == elevator_2f_button)
		{
			if (elevator_moving == true)
			{
				//아무런 동작안함
			}
			else if (current_floor == -1)
				showMessage("눌러도 아무런 반응이 없다.");
			else if (current_floor == 1 && stage_1_clear == false)
			{
				showMessage("고장난것 같다. 우선 수리를 해야할것 같다.");
			}
			else if (current_floor != 2) {
				playSound(elevator_button_sound);
				elevatorMove(2);
			}
		}
		else if (object == elevator_3f_button)
		{
			if (elevator_moving == true)
			{
				//아무런 동작안함
			}
			else if (current_floor == -1)
				showMessage("눌러도 아무런 반응이 없다.");
			else if (current_floor == 1 && stage_1_clear == false)
			{
				showMessage("고장난것 같다. 우선 수리를 해야할것 같다.");
			}
			else if (current_floor == 1 && stage_1_clear == true && stage_2_clear == false)
			{
				showMessage("눌러도 아무런 반응이 없다.");
			}
			else if (current_floor == 2 && stage_1_clear == true && stage_2_clear == false)
			{
				showMessage("배터리가 나간것 같다. 배터리를 충전해야 할것 같다.");
			}
			else if (zombie_appear == true && door15_closed[2]==false)
			{
				showMessage("이대로 돌아가기엔 위험한 것 같다.");
			}
			else if (current_floor != 3) {
				playSound(elevator_button_sound);
				elevatorMove(3);
			}
		}
		else if (object == elevator_plate)
		{
			enterSceneChangeCur(elevator_plate_scene);
		}
	}
	else if (cur_scene == elevator_plate_scene)
	{
		if (object == dir_elevator_plate_down)
		{
			enterSceneChangeCur(elevator_scene);
		}
		else if (object == elevator_screw[0][0]&& getHandObject() == laboratory_1f_driver)
		{
			hideObject(elevator_screw[0][0]);
			screw_removed[0][0] = true;
			elevator_plate_closed = !(screw_removed[0][0] * screw_removed[0][1] * screw_removed[1][0] * screw_removed[1][1]);
		}
		else if (object == elevator_screw[0][1] && getHandObject() == laboratory_1f_driver)
		{
			hideObject(elevator_screw[0][1] );
			screw_removed[0][1] = true;
			elevator_plate_closed = !(screw_removed[0][0] * screw_removed[0][1] * screw_removed[1][0] * screw_removed[1][1]);
		}
		else if (object == elevator_screw[1][0] && getHandObject() == laboratory_1f_driver)
		{
			hideObject(elevator_screw[1][0] );
			screw_removed[1][0] = true;
			elevator_plate_closed = !(screw_removed[0][0] * screw_removed[0][1] * screw_removed[1][0] * screw_removed[1][1]);
		}
		else if (object == elevator_screw[1][1] && getHandObject() == laboratory_1f_driver)
		{
			hideObject(elevator_screw[1][1]);
			screw_removed[1][1] = true;
			elevator_plate_closed = !(screw_removed[0][0] * screw_removed[0][1] * screw_removed[1][0] * screw_removed[1][1]);

		}
		else if (object == elevator_cable && getHandObject()== laboratory_1f_connector)
		{
			setObjectImage(elevator_cable, "Images/sw-connected.png");
			dropObject(laboratory_1f_connector);
			hideObject(laboratory_1f_connector);
			stage_1_clear = true;
			setSceneLight(elevator_scene,1.0f);
			setSceneLight(elevator_plate_scene, 1.0f);
			playSound(electro_sound);
		}
		else if (object == elevator_circuit && getHandObject() == elevator_battery && battery_empty == false)
		{
			dropObject(elevator_battery);
			hideObject(elevator_battery);
			showObject(elevator_battery_full);
			stage_2_clear = true;
			setSceneLight(elevator_scene, 1.0f);
			setSceneLight(elevator_plate_scene, 1.0f);
			playSound(electro_sound);

		}


		if (!elevator_plate_closed)
		{
			//나사 모두 빼고난후
			showObject(elevator_cable);
			showObject(elevator_battery);
			showObject(elevator_circuit);
		}

	}
	else if (cur_scene == screen_scene)
	{
		if (object == dir_sceen_down)
			enterSceneChangeCur(laboratory_scenes[9]);
	}



	// 공통 상호작용 
	if (object == laboratory_box_window_close)
	{
		playSound(manual_window_open_sound);
		hideObject(laboratory_box_window_close);
		showObject(laboratory_box_window_open);
	}
	else if (object == laboratory_box_window_open) {
		playSound(manual_window_close_sound);
		hideObject(laboratory_box_window_open);
		showObject(laboratory_box_window_close);
	}// 줍기
	else if (object == laboratory_1f_gmtPaper)
	{
		pickObject(laboratory_1f_gmtPaper);
	}
	else if (object == laboratory_3f_passwordPaper)
	{
		pickObject(laboratory_3f_passwordPaper);
	}
	else if (object == laboratory_2f_greenCard)
	{
		pickObject(laboratory_2f_greenCard);
	}
	else if (object == laboratory_2f_blackCard)
	{
		pickObject(laboratory_2f_blackCard);
	}
	else if (object == laboratory_3f_blackCard)
	{
		pickObject(laboratory_3f_blackCard);
	}
	else if (object == laboratory_2f_flask)
	{
		pickObject(laboratory_2f_flask);
	}
	else if (object == laboratory_1f_water)
	{
		pickObject(laboratory_1f_water);
	}
	else if (object == laboratory_1f_seed)
	{
		pickObject(laboratory_1f_seed);
	}
	else if (object == laboratory_3f_flower)
	{
		pickObject(laboratory_3f_flower);
	}
	else if (object == laboratory_2f_vaccine_recipe)
	{
		pickObject(laboratory_2f_vaccine_recipe);
	}
	else if (object == laboratory_2f_finalKey)
	{
		pickObject(laboratory_2f_finalKey);
	}
	else if (object == laboratory_1f_driver)
	{
		pickObject(laboratory_1f_driver);
	}
	else if (object == laboratory_1f_connector)
	{
		pickObject(laboratory_1f_connector);
	}
	else if (object == elevator_battery && battery_empty)
	{
		pickObject(elevator_battery);
	}
	else if (object == laboratory_2f_plug)
	{
		pickObject(laboratory_2f_plug);
	}
	else if (object == laboratory_3f_letter)
	{
		pickObject(laboratory_3f_letter);
	}
}
void objectCallback(ObjectID object, EventID e)
{
	if (object == laboratory_keypad_1f)
	{
		playSound(unlock_sound);
		setObjectImage(laboratory_safe_1f,"Images/safe-open.png");
		hideObject(laboratory_keypad_1f);
		
		showObject(laboratory_1f_seed);
		lock_opend[0] = true;
	}
	else if (object == laboratory_keypad_2f)
	{
		playSound(unlock_sound);
		setObjectImage(laboratory_safe_2f, "Images/safe-open.png");
		hideObject(laboratory_keypad_2f);

		showObject(laboratory_2f_blackCard);
		showObject(laboratory_2f_greenCard);
		lock_opend[1] = true;

	}
	else if (object == laboratory_keypad_3f)
	{
		playSound(unlock_sound);
		setObjectImage(laboratory_safe_3f, "Images/safe-open.png");
		hideObject(laboratory_keypad_3f);

		showObject(laboratory_3f_passwordPaper);
		showObject(laboratory_3f_blackCard);
		lock_opend[2] = true;

	}
	else if (object == laboratory_clock_1f)
	{
		playSound(unlock_sound);
		setObjectImage(laboratory_clock_safe_1f, "Images/safe-GMT-open.png");

		showObject(laboratory_1f_connector);
	}
	else if (object == laboratory_clock_2f)
	{
		playSound(unlock_sound);
		setObjectImage(laboratory_clock_safe_2f, "Images/safe-GMT-open.png");

		showObject(laboratory_2f_plug);
	}
	else if (object == laboratory_clock_3f)
	{
		playSound(unlock_sound);
		setObjectImage(laboratory_clock_safe_3f, "Images/safe-GMT-open.png");
		showObject(laboratory_3f_letter);
	}
	else if (e == EventID::EVENT_COMBINE_OBJECT )
	{
		vaccineMade=true;
	}
}

void enterSceneChangeCur(SceneID scene)
{
	enterScene(scene);
	cur_scene = scene;
}

void elevatorMove(int goal) {
	hideObject(dir_elevator_up);
	stopSound(current_bgm);
	playSound(elevator_move_sound);


	setSceneImage(elevator_scene,"images/scenes/elevator-closed.png");
	target_floor = goal;
	setTimer(elevator_move_timer,ELEVATOR_TIME);
	startTimer(elevator_move_timer);
	elevator_moving = true;
}

void setLaboratoryScene(int floor)
{
	char image[30];
	current_floor = floor;
	if (floor == 1)
	{
		for (int i = 0; i < NUM_OF_LABORATORY_SCENES; i++) {
			sprintf(image, "Images/scenes/past%d.png", i + 1);
			setSceneImage(laboratory_scenes[i], image);
		}
		setObjectImage(laboratory_plant_pot, "Images/plant-pot-1f.png");
		setObjectImage(laboratory_screen, "Images/screen1.png");

		showObject(laboratory_safe_1f);

		showObject(laboratory_1f_gmtPaper);
		//OBJECT 이미지 변환
		
		showObject(laboratory_clock_safe_1f);
		showObject(laboratory_clock_1f);
		showObject(laboratory_1f_driver);

		if (!lock_opend[0])
		{
			showObject(laboratory_keypad_1f);
		}
		if (!laboratory_1f_waterOnPot)
		{
			showObject(laboratory_1f_water);
		}
		if (!laboratory_1f_seedOnPot)
		{
			showObject(laboratory_1f_seedOnPot);
		}
	}
	else if (floor == 2)
	{
		for (int i = 0; i < NUM_OF_LABORATORY_SCENES; i++) {
			sprintf(image, "Images/scenes/present%d.png", i + 1);
			setSceneImage(laboratory_scenes[i], image);
		}
		setObjectImage(laboratory_plant_pot, "Images/plant-pot-2f.png");
		setObjectImage(laboratory_screen, "Images/screen2.png");

		//OBJECT 이미지 변환
		showObject(laboratory_safe_2f);
		showObject(laboratory_2f_flask);
		showObject(laboratory_clock_safe_2f);
		showObject(laboratory_clock_2f);
		showObject(laboratory_2f_cardTagger_15);
		showObject(laboratory_2f_cardTagger_16);
		showObject(laboratory_2f_cardTagger_chamber);
		showObject(laboratory_2f_vaccine_recipe);
		showObject(laboratory_2f_finalKey);
		showObject(laboratory_2f_charger);
		showObject(laboratory_2f_wire);



		if (laboratory_1f_waterOnPot && laboratory_1f_seedOnPot)
		{
			showObject(laboratory_2f_stem);
		}

		if (!lock_opend[1])
		{
			showObject(laboratory_keypad_2f);
		}
	}
	else if (floor == 3)
	{
		showObject(laboratory_3f_endingDoor);
		for (int i = 0; i < NUM_OF_LABORATORY_SCENES; i++) {
			sprintf(image, "Images/scenes/future%d.png", i + 1);
			setSceneImage(laboratory_scenes[i], image);

		}
		setObjectImage(laboratory_plant_pot, "Images/plant-pot-3f.png");
		setObjectImage(laboratory_screen, "Images/screen3.png");
		showObject(laboratory_3f_zombie_17);

		//OBJECT 이미지 변환
		showObject(laboratory_safe_3f);
		showObject(laboratory_clock_safe_3f);
		showObject(laboratory_clock_3f);
		showObject(laboratory_3f_cardTagger_15);
		showObject(laboratory_3f_cardTagger_16);
		if (laboratory_1f_waterOnPot && laboratory_1f_seedOnPot)
		{
			showObject(laboratory_3f_stem);
			showObject(laboratory_3f_flower);
		}

		if (!lock_opend[2])
		{
			showObject(laboratory_keypad_3f);
		}

	}
}

void showEscapeVideo()
{
	scene_frame = 0;
	setSceneImage(elevator_scene,"images/scenes/future18.png");
	setTimer(scene_change_timer, SCENE_FRAME_RATE);
	startTimer(scene_change_timer);
	//timer callback에서 이어짐
}


void hideLaboratoryObject(int floor)
{
	if (floor == 1)
	{
		hideObject(laboratory_1f_gmtPaper);
		hideObject(laboratory_safe_1f);
		hideObject(laboratory_keypad_1f);

		hideObject(laboratory_clock_safe_1f);
		hideObject(laboratory_clock_1f);
		hideObject(laboratory_1f_seed);
		hideObject(laboratory_1f_water);
		hideObject(laboratory_1f_driver);
	}
	else if (floor == 2)
	{
		hideObject(laboratory_safe_2f);
		hideObject(laboratory_keypad_2f);
		hideObject(laboratory_2f_flask);
		hideObject(laboratory_clock_safe_2f);
		hideObject(laboratory_clock_2f);
		hideObject(laboratory_2f_cardTagger_15);
		hideObject(laboratory_2f_cardTagger_16);
		hideObject(laboratory_2f_cardTagger_chamber);
		hideObject(laboratory_2f_stem);
		hideObject(laboratory_2f_vaccine_recipe);
		hideObject(laboratory_2f_finalKey);
		hideObject(laboratory_2f_charger);
		hideObject(laboratory_2f_wire);


	}
	else if (floor == 3)
	{
		hideObject(laboratory_safe_3f);
		hideObject(laboratory_keypad_3f);
		hideObject(laboratory_3f_endingDoor);
		hideObject(laboratory_clock_safe_3f);
		hideObject(laboratory_clock_3f);
		hideObject(laboratory_3f_cardTagger_15);
		hideObject(laboratory_3f_cardTagger_16);
		hideObject(laboratory_3f_stem);
		hideObject(laboratory_3f_flower);
		hideObject(laboratory_3f_zombie_17);
	}
	

}

void hideElevatorObject()
{
	hideObject(elevator_3f_button);
	hideObject(elevator_2f_button);
	hideObject(elevator_1f_button);
	hideObject(elevator_plate);
}
void showElevatorObject()
{
	showObject(elevator_3f_button);
	showObject(elevator_2f_button);
	showObject(elevator_1f_button);
	showObject(elevator_plate);

}
/*
-------------------------------------------------------------------------------------
|																					|
|								static process										|
|																					|
|																					|
-------------------------------------------------------------------------------------
*/

void scene_init()
{
	char image[30];

	for (int i = 0; i < NUM_OF_LABORATORY_SCENES; i++) {
		sprintf(image, "Images/scenes/past%d.png", i + 1);
		laboratory_scenes[i] = createScene("실험실", image);
	}
	elevator_scene = createScene("엘리베이터", "Images/scenes/elevator-closed.png");
	main_scene = createScene("???", "Images/scenes/elevator-closed.png");

	elevator_plate_scene = createScene("엘리베이터", "Images/scenes/elevator-plate.png");

	screen_scene = createScene("pc 화면", "Images/scenes/past-page.png");

}
void up_init(int i, int xpos, int ypos)
{
	dir_up[i] = createObject("Images/up.png");
	locateObject(dir_up[i], laboratory_scenes[i], xpos, ypos);
	showObject(dir_up[i]);
}

void down_init(int i, int xpos, int ypos)
{
	dir_down[i] = createObject("Images/down.png");
	locateObject(dir_down[i], laboratory_scenes[i], xpos, ypos);
	showObject(dir_down[i]);
}
void dir_init()
{
	for (int i = 0; i < NUM_OF_LABORATORY_SCENES; i++) {
		//17번 방 전까지 초기화 
		dir_left[i] = createObject("Images/left.png");
		locateObject(dir_left[i], laboratory_scenes[i], 0, 380);
		showObject(dir_left[i]);
		dir_right[i] = createObject("Images/right.png");
		locateObject(dir_right[i], laboratory_scenes[i], 1200, 380);
		showObject(dir_right[i]);
	}
	hideObject(dir_right[15]);
	hideObject(dir_right[16]);
	hideObject(dir_right[17]);
	hideObject(dir_left[17]);

	up_init(3, 300);
	up_init(5, 900);
	up_init(11, 300);
	up_init(13, 950);
	up_init(16, 630, 100);
	down_init(0);
	down_init(2);
	down_init(10);
	down_init(15);
	down_init(16);
	down_init(17);
	down_init(18);

	dir_elevator_up = createObject("Images/up.png");
	locateObject(dir_elevator_up, elevator_scene, 630, 20);

	dir_elevator_plate_down = createObject("Images/down.png");
	locateObject(dir_elevator_plate_down, elevator_plate_scene, 1000, 20);
	showObject(dir_elevator_plate_down);

	dir_sceen_down = createObject("Images/down.png");
	locateObject(dir_sceen_down, screen_scene, 600, 20);
	showObject(dir_sceen_down);

}
void sound_init()
{
	elevator_move_sound = createSound("Audios/elevator_move.wav");
	elevator_arrive_sound = createSound("Audios/elevator_ring.wav");
	elevator_button_sound = createSound("Audios/elevator_button.wav");
	elevator_blackout_sound = createSound("Audios/blackout.mp3");
	laboratory_1f_bgm = createSound("Audios/1f_bgm.mp3");
	laboratory_2f_bgm = createSound("Audios/2f_bgm.wav");
	laboratory_3f_bgm = createSound("Audios/3f_bgm.mp3");
	zombie_sound = createSound("Audios/zombie.mp3");
	confusion_sound = createSound("Audios/confusion.wav");
	auto_window_close_sound = createSound("Audios/auto_window_close.wav");
	auto_window_open_sound = createSound("Audios/auto_window_open.wav");
	manual_window_close_sound = createSound("Audios/manual_window_close.wav");
	manual_window_open_sound = createSound("Audios/manual_window_open.wav");
	soil_sound = createSound("Audios/soil.mp3");
	water_sound = createSound("Audios/water.mp3");
	generator_sound = createSound("Audios/generator.mp3");
	door_sound = createSound("Audios/door.mp3");
	unlock_sound = createSound("Audios/unlock.wav");
	breakDown_sound = createSound("Audios/breakDown.mp3");
	beep_sound = createSound("Audios/beep.mp3");
	electro_sound = createSound("Audios/electronic.wav");
}


void object_init()
{
	startButton = createObject("Images/start_button.png");
	locateObject(startButton,main_scene,580,200);
	showObject(startButton);

	//엘리베이터 버튼
	elevator_1f_button = createObject("Images/Button.png");
	locateObject(elevator_1f_button, elevator_scene, 1000, 420);

	elevator_3f_button = createObject("Images/Button.png");
	locateObject(elevator_3f_button, elevator_scene, 1000, 480);

	elevator_2f_button = createObject("Images/Button.png");
	locateObject(elevator_2f_button, elevator_scene, 1000, 450);

	elevator_plate = createObject("Images/elevator-plate.png");
	locateObject(elevator_plate, elevator_scene, 200, 500);


	// 엘리베이터 회로
	elevator_circuit = createObject("Images/elevator-circuit-full.png");
	locateObject(elevator_circuit,elevator_plate_scene,400,50);

	elevator_battery = createObject("Images/low-battery.png");
	locateObject(elevator_battery, elevator_plate_scene, 400,150);
	scaleObject(elevator_battery, 0.5f);

	elevator_battery_full = createObject("Images/full-battery.png");
	locateObject(elevator_battery_full, elevator_plate_scene, 400, 150);
	scaleObject(elevator_battery_full, 0.5f);


	elevator_cable = createObject("Images/sw-disconnected.png");
	locateObject(elevator_cable, elevator_plate_scene, 455, 595);

	//엘리베이터 나사
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
		{
			elevator_screw[i][j] = createObject("Images/screw.png");
			locateObject(elevator_screw[i][j], elevator_plate_scene, 400 + 360 * i, 50 + 580 * j);
			scaleObject(elevator_screw[i][j], 0.5f);
			showObject(elevator_screw[i][j]);
		}

	// 스크린

	laboratory_screen = createObject("Images/screen1.png");
	locateObject(laboratory_screen,laboratory_scenes[9],668,252);
	showObject(laboratory_screen);

	//BOX화분
	laboratory_plant_pot = createObject("Images/plant-pot-1f.png");
	locateObject(laboratory_plant_pot, laboratory_scenes[2], 565, 290);
	showObject(laboratory_plant_pot);

	laboratory_2f_stem = createObject("Images/plant2.png");
	locateObject(laboratory_2f_stem, laboratory_scenes[2], 585, 350);
	laboratory_3f_stem = createObject("Images/plant3.png");
	locateObject(laboratory_3f_stem, laboratory_scenes[2], 560, 350);
	laboratory_3f_flower = createObject("Images/flower.png");
	locateObject(laboratory_3f_flower, laboratory_scenes[2], 570, 400);

	laboratory_box_window_close= createObject("Images/box-window-close.png");
	locateObject(laboratory_box_window_close, laboratory_scenes[2], 500, 250);
	showObject(laboratory_box_window_close);


	laboratory_box_window_open = createObject("Images/box-window-open.png");
	locateObject(laboratory_box_window_open, laboratory_scenes[2], 450, 270);

	//챔버 플라스크, 마지막 문 키
	laboratory_2f_finalKey = createObject("Images/key.png");
	locateObject(laboratory_2f_finalKey, laboratory_scenes[7], 480, 250);

	laboratory_2f_flask = createObject("Images/flask.png");
	locateObject(laboratory_2f_flask, laboratory_scenes[7], 480, 250);

	laboratory_chamber_window_close = createObject("Images/chamber_window.png");
	locateObject(laboratory_chamber_window_close, laboratory_scenes[7], 370, 220);
	showObject(laboratory_chamber_window_close);

	laboratory_chamber_window_open = createObject("Images/chamber_window.png");
	locateObject(laboratory_chamber_window_open, laboratory_scenes[7], 135, 220);

	laboratory_2f_cardTagger_chamber = createObject("Images/card-tagger.png");
	locateObject(laboratory_2f_cardTagger_chamber, laboratory_scenes[7], 630, 300);


	//최종문
	laboratory_3f_endingDoor = createObject("Images/door.png");
	locateObject(laboratory_3f_endingDoor, laboratory_scenes[18], 400, -50);


	//일반 금고
	//1층
	laboratory_safe_1f = createObject("Images/safe-close.png");
	locateObject(laboratory_safe_1f, laboratory_scenes[1], 500, -30);

	laboratory_1f_seed = createObject("Images/seed.png");
	locateObject(laboratory_1f_seed, laboratory_scenes[1], 550, 50);

	//2층
	laboratory_safe_2f = createObject("Images/safe-close.png");
	locateObject(laboratory_safe_2f, laboratory_scenes[1], 500, -30);

	laboratory_2f_blackCard = createObject("Images/black_card.png");
	locateObject(laboratory_2f_blackCard, laboratory_scenes[1], 550, 50);
	scaleObject(laboratory_2f_blackCard, 0.5f);
	laboratory_2f_greenCard = createObject("Images/green_card.png");
	locateObject(laboratory_2f_greenCard, laboratory_scenes[1], 600, 70);
	scaleObject(laboratory_2f_greenCard, 0.5f);

	//3층

	laboratory_safe_3f = createObject("Images/safe-close.png");
	locateObject(laboratory_safe_3f, laboratory_scenes[1], 500, -30);

	laboratory_3f_blackCard = createObject("Images/black_card.png.");
	locateObject(laboratory_3f_blackCard, laboratory_scenes[1], 550, 50);
	scaleObject(laboratory_3f_blackCard, 0.5f);
	laboratory_3f_passwordPaper = createObject("Images/password-paper.png");
	locateObject(laboratory_3f_passwordPaper, laboratory_scenes[1], 550, 50);
	scaleObject(laboratory_3f_passwordPaper, 0.1f);


	//일반금고 키패드
	laboratory_keypad_1f = createObject("Images/keypad.png");
	locateObject(laboratory_keypad_1f, laboratory_scenes[1], 617, 80);

	laboratory_keypad_2f = createObject("Images/keypad.png");
	locateObject(laboratory_keypad_2f, laboratory_scenes[1], 617, 80);

	laboratory_keypad_3f = createObject("Images/keypad.png");
	locateObject(laboratory_keypad_3f, laboratory_scenes[1], 617, 80);

	//gmt 종이, gmt 금고
	laboratory_1f_gmtPaper = createObject("Images/gmt.png");
	locateObject(laboratory_1f_gmtPaper, laboratory_scenes[18], 700, 300);
	scaleObject(laboratory_1f_gmtPaper,0.2f);

	//1층
	laboratory_clock_safe_1f = createObject("Images/safe-gmt-close.png");
	locateObject(laboratory_clock_safe_1f, laboratory_scenes[8], 80, 220);
	laboratory_clock_1f = createObject("Images/clock.png");
	locateObject(laboratory_clock_1f, laboratory_scenes[8], 140, 420);

	laboratory_1f_connector = createObject("Images/wire.png");
	locateObject(laboratory_1f_connector, laboratory_scenes[8], 140,270);
	scaleObject(laboratory_1f_connector, 0.8f);

	//2층
	laboratory_clock_safe_2f = createObject("Images/safe-gmt-close.png");
	locateObject(laboratory_clock_safe_2f, laboratory_scenes[8], 80, 220);
	laboratory_clock_safe_3f = createObject("Images/safe-gmt-close.png");
	locateObject(laboratory_clock_safe_3f, laboratory_scenes[8], 80, 220);

	laboratory_3f_letter = createObject("Images/letter.png");
	locateObject(laboratory_3f_letter, laboratory_scenes[8], 80, 220);
	scaleObject(laboratory_3f_letter, 0.1f);



	laboratory_2f_plug = createObject("Images/plug.png");
	locateObject(laboratory_2f_plug, laboratory_scenes[8], 140, 270);
	scaleObject(laboratory_2f_plug, 0.8f);


	laboratory_clock_2f = createObject("Images/clock.png");
	locateObject(laboratory_clock_2f, laboratory_scenes[8], 140,420);

	laboratory_clock_3f = createObject("Images/clock.png");
	locateObject(laboratory_clock_3f, laboratory_scenes[8], 140, 420);

	// 2,3층 문 카드 태깅

	laboratory_2f_cardTagger_15 = createObject("Images/card-tagger.png");
	locateObject(laboratory_2f_cardTagger_15, laboratory_scenes[15], 325, 300);
	laboratory_2f_cardTagger_16 = createObject("Images/card-tagger.png");
	locateObject(laboratory_2f_cardTagger_16, laboratory_scenes[16], 280, 300);

	laboratory_3f_cardTagger_15 = createObject("Images/card-tagger.png");
	locateObject(laboratory_3f_cardTagger_15, laboratory_scenes[15], 325, 300);
	laboratory_3f_cardTagger_16 = createObject("Images/card-tagger.png");
	locateObject(laboratory_3f_cardTagger_16, laboratory_scenes[16], 280, 300);

	// 1층 물
	laboratory_1f_water = createObject("Images/water.png");
	locateObject(laboratory_1f_water, laboratory_scenes[4],150,350);
	scaleObject(laboratory_1f_water, 0.4f);
	
	//백신, 제조법
	vaccine = createObject("Images/vaccine.png");
	laboratory_2f_vaccine_recipe = createObject("Images/vaccine_recipe.jpg");
	locateObject(laboratory_2f_vaccine_recipe, laboratory_scenes[18], 700, 300);
	scaleObject(laboratory_2f_vaccine_recipe, 0.2f);

	//드라이버
	laboratory_1f_driver =createObject("Images/driver.png");
	locateObject(laboratory_1f_driver, laboratory_scenes[17], 300, 250);
	scaleObject(laboratory_1f_driver, 0.2f);

	//충전기, 전선
	laboratory_2f_wire = createObject("Images/socket-disconnect-unplug.png");
	locateObject(laboratory_2f_wire, laboratory_scenes[17], 800, 200);
	scaleObject(laboratory_2f_wire, 0.5f);

	laboratory_2f_charger = createObject("Images/charger.png");
	locateObject(laboratory_2f_charger, laboratory_scenes[17], 700, 100);
	scaleObject(laboratory_2f_charger, 0.5f);

	//좀비
	laboratory_3f_zombie_17 = createObject("Images/zombie.png");
	locateObject(laboratory_3f_zombie_17, laboratory_scenes[17],100,100);

	video_zombie = createObject("Images/zombie.png");
	locateObject(video_zombie, elevator_scene, 100, 100);
}

void timer_init()
{
	elevator_move_timer = createTimer(ELEVATOR_TIME);
	elevator_open_timer = createTimer(3.0f);
	scene_change_timer = createTimer(0.5f);
	ending_timer = createTimer(5.0f);
}

