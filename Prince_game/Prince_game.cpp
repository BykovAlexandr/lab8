

#include <iostream> 
#include <SFML/Graphics.hpp>
#include "map.h" 


using namespace sf;

////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player { // класс Игрока
public:
	float x, y, w, h, dx, dy, speed = 0; //координаты игрока х и у, высота ширина, ускорение (по х и по у), сама скорость
	int dir = 0; //направление (direction) движения игрока
	String File; //файл с расширением
	Image image;//сфмл изображение
	Texture texture;//сфмл текстура
	Sprite sprite;//сфмл спрайт

	Player(String F, float X, float Y, float W, float H) {  //Конструктор с параметрами(формальными) для класса Player. При создании объекта класса мы будем задавать имя файла, координату Х и У, ширину и высоту
		File = F;//имя файла+расширение
		w = W; h = H;//высота и ширина
		image.loadFromFile("images/" + File);//запихиваем в image наше изображение вместо File мы передадим то, что пропишем при создании объекта. В нашем случае "hero.png" и получится запись идентичная 	image.loadFromFile("images/hero/png");
		image.createMaskFromColor(Color(41, 33, 59));//убираем ненужный темно-синий цвет, эта тень мне показалась не красивой.
		texture.loadFromImage(image);//закидываем наше изображение в текстуру
		sprite.setTexture(texture);//заливаем спрайт текстурой
		x = X; y = Y;//координата появления спрайта
		sprite.setTextureRect(IntRect(0, 0, 11, 40));  //Задаем спрайту один прямоугольник для вывода одного льва, а не кучи львов сразу. IntRect - приведение типов
	}



	void update(float time) //функция "оживления" объекта класса. update - обновление. принимает в себя время SFML , вследствие чего работает бесконечно, давая персонажу движение.
	{
		switch (dir)//реализуем поведение в зависимости от направления. (каждая цифра соответствует направлению)
		{
		case 0: dx = speed; dy = 0;   break;//по иксу задаем положительную скорость, по игреку зануляем. получаем, что персонаж идет только вправо
		case 1: dx = -speed; dy = 0;   break;//по иксу задаем отрицательную скорость, по игреку зануляем. получается, что персонаж идет только влево
		case 2: dx = 0; dy = speed;   break;//по иксу задаем нулевое значение, по игреку положительное. получается, что персонаж идет только вниз
		case 3: dx = 0; dy = -speed;   break;//по иксу задаем нулевое значение, по игреку отрицательное. получается, что персонаж идет только вверх
		}

		x += dx * time;//то движение из прошлого урока. наше ускорение на время получаем смещение координат и как следствие движение
		y += dy * time;//аналогично по игреку

		speed = 0;//зануляем скорость, чтобы персонаж остановился.
		sprite.setPosition(x, y); //выводим спрайт в позицию x y , посередине. бесконечно выводим в этой функции, иначе бы наш спрайт стоял на месте.
	}
};


int main()
{
	RenderWindow window(sf::VideoMode(640, 480), "Lesson 9. kychka-pc.ru");

	Image map_image;//объект изображения для карты
	map_image.loadFromFile("images/mapforprince.png");//загружаем файл для карты
	Texture map;//текстура карты
	map.loadFromImage(map_image);//заряжаем текстуру картинкой
	Sprite s_map;//создаём спрайт для карты
	s_map.setTexture(map);//заливаем текстуру спрайтом

	float CurrentFrame = 0;//хранит текущий кадр
	Clock clock;

	Player hero("hero.png", 250, 250, 96.0, 96.0);//создаем объект p класса player,задаем "hero.png" как имя файла+расширение, далее координата Х,У, ширина, высота.

	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;


		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}


		///////////////////////////////////////////Управление персонажем с анимацией////////////////////////////////////////////////////////////////////////
		if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) {
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
		if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) {
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
				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(55, 60, 32, 32)); //если встретили символ пробел, то рисуем 1й квадратик
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(0, 569, 32, 32));//если встретили символ s, то рисуем 2й квадратик
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(190, 130, 32, 32));//если встретили символ 0, то рисуем 3й квадратик


				s_map.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат

				window.draw(s_map);//рисуем квадратики на экран
			}
		window.draw(hero.sprite);//рисуем спрайт объекта p класса player
		window.display();
	}

	return 0;
}