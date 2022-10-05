#include <string>
#include <iomanip>
#include <iostream>
#include <bitset>
#include <random>

using namespace std;

void printMessage() {
	cout << "\nВыберите операцию, которую хотите произвести с хэш-таблицей:\n" <<
		"1. Добавить элемент в таблицу\n" <<
		"2. Удалить элемент из таблицы по ключу\n" <<
		"3. Найти элемент в таблице по ключу\n" <<
		"4. Вывести текущую таблицу\n" <<
		"5. Изменить размер таблицы\n" <<
		"6. Сгенерировать N (5-7 желательно) случайных записей для таблицы\n" <<
		"0. Закончить работу приложения\n";
}

struct elemTable {
private:
	int code = 0; // Код товара, 6-разрядный
	string name; // Название товара
	float price; // Цена товара
	bool isOpen = true;
	// Элемент таблицы - товар
public:
	// Конструктор
	elemTable(int _code, string _name, float _price) : code(_code), name(_name), price(_price) {}
	elemTable() { // Конструктор по умолчанию
		code = 0;
		name = "";
		price = 0;
	} 
	void openOrClose() { // Метод для смены состояния
		isOpen = !isOpen;
	}
	void print() { // Вывод значений полей элемента
		cout << "\t" << code << ": " << name << " " << price << endl;
	}
	bool canWrite() { // Получение состояния ячейки
		return isOpen;
	}
	int getCode() { // Получение кода товара
		return code;
	}
	string getName() { // Получение названия товара
		return name;
	}
	float getPrice() { // Получение цены товара
		return price;
	}
};

class hashTable {
private:
	elemTable* table; // Указатель на множество 
	int curTableLength = 0;
public: 
	hashTable(int tableLength) { // Конструктор
		table = new elemTable[tableLength];
		curTableLength = tableLength;
	}
	int hashFunctionHorner(const int _code) { // Общая хэш-функция
		return (_code) % curTableLength;
	}
	int hashFunc1(int _code) { // Первая хэш-функция с дикрементом
		return hashFunctionHorner(_code - 1);
 	}
	int hashFunc2(int _code) { // Вторая хэш-функция с инкрементом
		return hashFunctionHorner(_code + 1);
	}
	bool add(int _code, string _name, float _price) { // Метод добавления нового элемента
		int h1 = hashFunc1(_code);
		int h2 = hashFunc2(_code);
		int index = 0;
		index = h1 % curTableLength;
		if (isOpen(index)) {
			table[index] = elemTable(_code, _name, _price);
			table[index].openOrClose();
			return true;
		}
		else { // Рехеширование при коллизии
			cout << "\t\t<Возникла коллизия! Пытаемся найти другую свободную ячейку...>\n";
			for (int i = 1; i < curTableLength; i++) {
				index = (h1 + i * h2) % curTableLength;
				if (isOpen(index)) {
					table[index] = elemTable(_code, _name, _price);
					table[index].openOrClose();
					cout << "\t\t<Найдена подходящая ячейка. Коллизия предотвращена!>\n";
					return true;
				}
			}
		}
		return false;
	};
	void showAllTable() { // Метод вывода всех заполненных ячеек таблицы
		int j = 0;
		for (int i = 0; i < curTableLength; i++) {
			if (!table[i].canWrite()) {
				table[i].print();
				j++;
			}
		}
		if (!j) {
			cout << "\t      Таблица пока что пуста!" << endl;
			return;
		}
		cout << "\tТекущее количество записей в \n\tтаблице: " << j << endl;
	}
	elemTable find(int _code) { // Метод поиска по ключу
		int h1 = hashFunc1(_code);
		int h2 = hashFunc2(_code);
		for (int i = 0; i < curTableLength; ++i) {
			if (table[h1 + i*h2].getCode() == _code)
				return table[h1];
		}
		return elemTable();
	}
	elemTable remove(int _code) {
		int h1 = hashFunc1(_code);
		int h2 = hashFunc2(_code);
		for (int i = 0; i < curTableLength; ++i) {
			if (table[h1 + i * h2].getCode() == _code) {
				elemTable t = table[h1 + i * h2];
				table[h1 + i * h2] = elemTable();
				return t;
			}
		}
		return elemTable();
	}
	bool isOpen(int index) { // Проверка на возможность записи в ячейку
		return table[index].canWrite();
	}
	elemTable* resizeTable(const int newSize) { // Метод расширения таблицы
		elemTable* newTable = new elemTable[newSize];
		for (int i = 0; i < newSize; i++) {
			if (i >= curTableLength)
				newTable[i] = elemTable();
			else
				newTable[i] = this->table[i];
		}
		delete[] this->table;
		this->table = new elemTable[newSize];
		for (int i = 0; i < newSize; i++) {
			this->table[i] = newTable[i];
		}
		delete[] newTable;
		curTableLength = newSize;
		return table;
	}

	int getSize() { // Метод получения размера массива элементов
		return curTableLength;
	}
	void generateRandom(int count) { // Метод генерации случайных значений в количестве count
		srand(time(NULL));
		int _code, index;
		int h1, h2;
		string b;
		bool flag = true;
		for (int j = 0; j < count; j++) {
			flag = true;
			while (flag) { // Создание ключа и проверка на его уникальность
				_code = rand();
				while (_code < 100000)
					_code <<= 2;
				while (_code > 999999)
					_code >>= 2;
				for (int i = 0; i < curTableLength; ++i) {
					h1 = hashFunc1(_code);
					h2 = hashFunc2(_code);
					if (table[(h1 + i * j * h2) % curTableLength].getCode() == _code)
						break; // Если такой ключ уже существует
					if (i == count - 1) // Если проверены все ключи и не найден дубликат
						flag = false;
				}
			} // После проверки создания и проверки, генерируем строку товара
			for (int i = 0; i < 1 + (rand() % 100); i++) {
				if (i == 0) // Первый символ будет заглавным
					b += ('A' + rand() % 26);
				else // последующие - строчными
					b += ('a' + rand() % 26);
				if (i > 16) // Пускай и диапазон итераций выбран от 1 до 100 (для выборки),
						   // строки рассматриваем длины 16 и меньше.
					break;
			}
			add(_code, b, rand()); // Добавляем сгенерированный элемент в таблицу.
			b = ""; // Очищаем строку с названием товара
		}
	}
};

int main() {
	// Вариант 5. Открытая адресация (двойное хеширование)
	setlocale(LC_ALL, "Rus");
	unsigned int size;
	cout << "Приветствую!\nДля работы с хэш-таблицей требуется ввести ее размеры. "
		<< "Введите положительное число\nN: ";
	cin >> size;
	hashTable* table = new hashTable(size);
	bool flag = true;
	int code;
	string name;
	float price;
	elemTable et;
	int fresh = 0;
	int randCount = 0;
	int commandNumber = 0;
	while (flag) {
		printMessage();
		cin >> commandNumber;
		switch (commandNumber) {
		case 1:
			cout << "\tДля добавления значения в таблицу, введите следующие значения:\n" <<
				"\t\tКод товара: ";
			cin >> code;
			cout << "\t\tНазвание товара: ";
			cin >> name;
			cout << "\t\tЦена (вещественное число): ";
			cin >> price;
			if (table->add(code, name, price)) {
				cout << "\tЭлемент успешно добавлен!" << endl;
				break;
			}
			cout << "\tВидимо, что-то пошло не так... Элемент не был добавлен в таблицу" << endl;
			break;
		case 2:
			cout << "\tДля удаления элемента из таблицы, введите его код: ";
			cin >> code;
			if (table->remove(code).getName() != "") {
				cout << "\tЭлемент успешно удален!" << endl;
				break;
			}
			cout << "\tВидимо, элемента с данным значением кода товара нет в таблице" << endl;;
			break;
		case 3:
			cout << "\tДля нахождения товара, введите его код: ";
			cin >> code;
			et = table->find(code);
			if (et.getName() != "") {
				cout << "\tЭлемент успешно найден!" << endl;
				cout << "\t\tКод: " << et.getCode() <<
					"\n\t\tНазвание: " << et.getName() <<
					"\n\t\tЦена: " << et.getPrice() << endl;
				break;
			}
			cout << "\tВидимо, элемента с заданным значением кода товара нет в таблице" << endl;
			break;
		case 4:
			cout << endl << "\t----------------------------------" << endl;
			table->showAllTable();
			cout << "\t----------------------------------" << endl;
			break;
		case 5:
			cout << "\tТекущий размер таблицы: " << table->getSize() << endl;
			cout << "\tВведите новый размер таблицы, превыщающий предыдущий: ";
			cin >> fresh;
			if (table->getSize() >= fresh) {
				cout << "\tВидимо, новый размер таблицы меньше предыдущего. Операция некорректна" << endl;
				break;
			}
			table->resizeTable(fresh);
			cout << "\tРазмер таблицы изменен. Текущий размер таблицы: " << table->getSize() << endl;
			break;
		case 6:
			cout << "\t(Примечание: если количество записей привысит количество\n" <<
				"\tсвободных ячеек, то программа запишет лишь допустимое количество)\n" <<
				"\tВведите количество записей, которое хотите сгенерировать: ";
			cin >> randCount;
			table->generateRandom(randCount);
			break;
		case 0:
			cout << "Завершаем работу программы..." << endl;
			flag = false;
			break;
		default:
			cout << "Введено некорректное значение. Попробуйте ввести номер команды из списка";
			break;
		}
	}
}
