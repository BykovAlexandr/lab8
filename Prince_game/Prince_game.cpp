

#include <SFML/Graphics.hpp>
#include "map.h"
#include <iostream>
#include <sstream>

using namespace sf;

////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player {

public:
	float w, h, dx, dy, x, y, speed;
	int dir, playerScore, health;
	bool life, isMove, isSelect, onGround;//добавили переменные состояния нахождения на земле
	enum { left, right, up, down, jump, stay } state;//добавляем тип перечисления - состояние объекта
	String File;
	Image image;
	Texture texture;
	Sprite sprite;
	Player(String F, float X, float Y, float W, float H) {

		dir = 0; speed = 0; playerScore = 0; health = 100; dx = 0; dy = 0;
		life = true; isMove = false; isSelect = false; onGround = false;
		File = F;
		w = W; h = H;
		image.loadFromFile("images/" + File);
		image.createMaskFromColor(Color(41, 33, 59));
		texture.loadFromImage(image);
		sprite.setTexture(texture);

		x = X; y = Y;
		sprite.setTextureRect(IntRect(0, 0, 11, 40));
		sprite.setOrigin(w / 2, h / 2);
	}
	void control() {
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			state = left;
			speed = 0.1;
			//currentFrame += 0.005*time;
			//if (currentFrame > 3) currentFrame -= 3;
			//p.sprite.setTextureRect(IntRect(96 * int(currentFrame), 135, 96, 54));
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			state = right;
			speed = 0.1;
			//	currentFrame += 0.005*time;
			//	if (currentFrame > 3) currentFrame -= 3;
			//	p.sprite.setTextureRect(IntRect(96 * int(currentFrame), 232, 96, 54));
		}

		if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onGround)) {
			state = jump; dy = -0.5; onGround = false;//то состояние равно прыжок,прыгнули и сообщили, что мы не на земле
			//currentFrame += 0.005*time;
			//if (currentFrame > 3) currentFrame -= 3;
			//p.sprite.setTextureRect(IntRect(96 * int(currentFrame), 307, 96, 96));
		}

	}
	void update(float time)
	{
		control();//функция управления персонажем
		switch (state)//тут делаются различные действия в зависимости от состояния
		{
		case right: dx = speed; break;//состояние идти вправо
		case left: dx = -speed; break;//состояние идти влево
		case up: break;//будет состояние поднятия наверх (например по лестнице)
		case down: break;//будет состояние во время спуска персонажа (например по лестнице)
		case jump: break;//здесь может быть вызов анимации
		case stay: break;//и здесь тоже		
		}
		x += dx * time;
		checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
		y += dy * time;
		checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
		if (!isMove) speed = 0;
		sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
		if (health <= 0) { life = false; }
		dy = dy + 0.0015 * time;//делаем притяжение к земле
	}

	float getplayercoordinateX() {
		return x;
	}
	float getplayercoordinateY() {
		return y;
	}

	void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
	{
		for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
			for (int j = x / 32; j < (x + w) / 32; j++)
			{
				if (TileMap[i][j] == '0')//если элемент наш тайлик земли? то
				{
					if (Dy > 0) { y = i * 32 - h;  dy = 0; onGround = true; }//по Y вниз=>идем в пол(стоим на месте) или падаем. В этот момент надо вытолкнуть персонажа и поставить его на землю, при этом говорим что мы на земле тем самым снова можем прыгать
					if (Dy < 0) { y = i * 32 + 32;  dy = 0; }//столкновение с верхними краями карты(может и не пригодиться)
					if (Dx > 0) { x = j * 32 - w; }//с правым краем карты
					if (Dx < 0) { x = j * 32 + 32; }// с левым краем карты
				}
			}
	}
};


int main()
{
	RenderWindow window(sf::VideoMode(1920, 1080), "Lesson 9. kychka-pc.ru");

	Image map_image;//объект изображения для карты
	map_image.loadFromFile("images/mapforprince.png");//загружаем файл для карты
	Texture map;//текстура карты
	map.loadFromImage(map_image);//заряжаем текстуру картинкой
	Sprite s_map;//создаём спрайт для карты
	s_map.setTexture(map);//заливаем текстуру спрайтом

	float CurrentFrame = 0;//хранит текущий кадр
	Clock clock;

	Player hero("hero.png", 64, 32, 32, 42);//создаем объект p класса player,задаем "hero.png" как имя файла+расширение, далее координата Х,У, ширина, высота.

	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 600;


		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}


		///////////////////////////////////////////Управление персонажем с анимацией////////////////////////////////////////////////////////////////////////
		if ((Keyboard::isKeyPressed(Keyboard::Left))) {
			hero.dir = 1; hero.speed = 0.1;//dir =1 - направление вверх, speed =0.1 - скорость движения. Заметьте - время мы уже здесь ни на что не умножаем и нигде не используем каждый раз
			CurrentFrame += 0.005 * time;
			if (CurrentFrame > 4) CurrentFrame -= 4; //проходимся по кадрам с первого по третий включительно. если пришли к третьему кадру - откидываемся назад.
			if (CurrentFrame < 2)
				hero.sprite.setTextureRect(IntRect(1092 - (32 * int(CurrentFrame)), 102, 30, 42)); //проходимся по координатам Х. получается 96,96*2,96*3 и опять 96
			if (int(CurrentFrame) == 2)
				hero.sprite.setTextureRect(IntRect(1036, 102, 24, 42)); //проходимся по координатам Х. получается 96,96*2,96*3 и опять 96
			if (int(CurrentFrame) == 3)
				hero.sprite.setTextureRect(IntRect(1012, 102, 24, 42)); //проходимся по координатам Х. получается 96,96*2,96*3 и опять 96
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			hero.dir = 0; hero.speed = 0.1;//направление вправо, см выше
			CurrentFrame += 0.005 * time;
			if (CurrentFrame > 4) CurrentFrame -= 4; //проходимся по кадрам с первого по третий включительно. если пришли к третьему кадру - откидываемся назад.
			if (int(CurrentFrame) == 0)
				hero.sprite.setTextureRect(IntRect(318, 102, 30, 42)); //проходимся по координатам Х. получается 96,96*2,96*3 и опять 96
			if (int(CurrentFrame) == 1)
				hero.sprite.setTextureRect(IntRect(348, 102, 34, 42)); //проходимся по координатам Х. получается 96,96*2,96*3 и опять 96
			if (int(CurrentFrame) == 2)
				hero.sprite.setTextureRect(IntRect(380, 102, 24, 42)); //проходимся по координатам Х. получается 96,96*2,96*3 и опять 96
			if (int(CurrentFrame) == 3)
				hero.sprite.setTextureRect(IntRect(404, 102, 24, 42)); //проходимся по координатам Х. получается 96,96*2,96*3 и опять 96
		}




		hero.update(time);//оживляем объект p класса Player с помощью времени sfml, передавая время в качестве параметра функции update. благодаря этому персонаж может двигаться


		window.clear();


		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{	
				if (TileMap[i][j] == 'h')  s_map.setTextureRect(IntRect(700,265, 32, 32)); //если встретили символ h, то рисуем гнилой квадратик
				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(55, 60, 32, 32)); //если встретили символ пробел, то рисуем 1й квадратик
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(0, 569, 32, 32));//если встретили символ s, то рисуем факел
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(190, 130, 32, 32));//если встретили символ 0, то рисуем 2й квадратик


				s_map.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат

				window.draw(s_map);//рисуем квадратики на экран
			}
		window.draw(hero.sprite);//рисуем спрайт объекта p класса player
		window.display();
	}

	return 0;
}