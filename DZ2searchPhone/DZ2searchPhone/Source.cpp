#include<iostream>
#include<Windows.h>
#include<cstring>
#include<fstream>

using namespace std;

//створюю структуру, яка зберігатиме клієнтів
struct Client
{
	char name[80];
	char phone[20];
};

//Прототипи функцій 
void menu();
void writeClientTofile();
void readDisplayClients();
void editClient();//редагуємо інфу про клієнта
void deleteClient(); // Додана функція видалення
void searchClientByPhone(); // Нова функція для пошуку


const char* fileStorage = "client.txt";// файл який зберігає кліентів

int main()
{
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);
	cout << "----Робота з файлами--- \n";
	menu();

}

void menu()
{
	int action = 0;
	do
	{
		cout << "Оберіть операцію:\n";
		cout << "0.Вихід\n";
		cout << "1.Додати клієнта\n";
		cout << "2.Відобразити клієнтів\n";
		cout << "3.Змінити клієнта\n";
		cout << "4. Видалити клієнта\n"; // Доданий пункт меню
		cout << "5. Пошук за номером телефону\n"; // Додано пункт меню
		cout << "->_";
		cin >> action;

		switch (action)
		{
		case 1:
			cin.ignore();//cin.action - видаляємо 
			writeClientTofile();//Викликаємо функццію \n"
			break;
		case 2:
			//cout << "-------Читаємо список клієнтів--------\n";
			readDisplayClients();
			break;
		case 3:
			//cout << "-------Зміна клієнтів--------\n";
			editClient();
			break;
		case 4:
			deleteClient(); // Виклик функції видалення
			break;
		case 5:
			cin.ignore(); // Очищення буфера перед введенням рядка пошуку
			searchClientByPhone(); // Виклик функції пошуку
			break;

		}

	} while (action != 0);
}

void writeClientTofile()
{
	Client client;
	cout << "Вкажіть ПІБ клієнта: \n";
	cin.getline(client.name, 80, '\n');
	cout << "Вкажіть телефон клієнта: \n";
	cin.getline(client.phone, 20, '\n');
	ofstream outFile(fileStorage, ios_base::app);//ios_base::app - для дозапису
	outFile << client.name << "\n";
	outFile << client.phone << "\n";
	outFile.close();
}

void readDisplayClients()
{
	ifstream readerFile(fileStorage);
	bool isOpen = readerFile.is_open();
	if (!isOpen)
	{
		cout << "Файл не існує " << fileStorage << "\n";
		return;
	}
	Client marko;
	int count = 0;
	while (readerFile.getline(marko.name, 80, '\n'))
	{
		if (strlen(marko.name) == 0)
		{
			return;
		}
		readerFile.getline(marko.phone, 20, '\n');
		cout << "----" << ++count << "----\n";
		cout << "Name: " << marko.name << "\nPhone: " << marko.phone << endl;
	}
	readerFile.close();
}

void editClient()
{
	//1.Користувач вказує номер запису.
	//2.Знаходжу даний запис і зберігаю його у структуру.
	//3.Даю можливіть користувачу ввести нові дані
	//4.Поновіню оновляю файл із усі користувачами і записую оновленого клієнта
	//Підраховую кількість клієнтів
	ifstream readerFile(fileStorage);
	bool isOpen = readerFile.is_open();
	if (!isOpen)
	{
		cout << "Файл не існує " << fileStorage << "\n";
		return;
	}
	Client marko;
	int count = 0;
	while (readerFile.getline(marko.name, 80, '\n'))
	{
		if (strlen(marko.name) == 0)
		{
			break; //виходимо з циклу
		}
		readerFile.getline(marko.phone, 20, '\n');
		count++; //піраховую кількість клієнтів
	}

	int numberClient;
	cout << "Вкажіть номер клієнта, якого потрібно змінити\n";
	cout << "->_";
	cin >> numberClient;

	if (numberClient<0 || numberClient > count)
	{
		cout << "Номер клієнта вказано не вірно!\n";
		return;
	}

	//Якщо ми знаємо кількість клієнтів ми можемо виділити память під дану кількість
	Client* list = new Client[count];
	//вичитуємо із файлу усіх клієнтів
	//Тут є проблема. Ми коли читати файл у ньому змістилися до кінця. 
	// потрібно перемітитися на початок
	readerFile.clear();
	readerFile.seekg(0); //змістився на початок файлу
	for (int i = 0; i < count; i++)
	{
		readerFile.getline(list[i].name, 80, '\n');
		readerFile.getline(list[i].phone, 80, '\n');
	}
	readerFile.close();

	cout << "Вкажіть ім'я(" << list[numberClient - 1].name << "): ";

	cin.ignore(); //очищаємо консоль \n
	cin.getline(marko.name, 80, '\n');
	if (strlen(marko.name) != 0)
		strcpy_s(list[numberClient - 1].name, marko.name); //Якщо вказав ім'я то ми його змінюємо у списку

	cout << "Вкажіть телефон(" << list[numberClient - 1].phone << "): ";
	cin.getline(marko.phone, 80, '\n');
	if (strlen(marko.phone) != 0)
		strcpy_s(list[numberClient - 1].phone, marko.phone); //Якщо вказав телефон то ми його змінюємо у списку

	//Тепер можна сміливо очисти файл і запити туди оновлені дані
	ofstream outUpdateClients(fileStorage); //воно його автомаично очистить, бо він відривається для запису
	for (int i = 0; i < count; i++)
	{
		outUpdateClients << list[i].name << '\n';
		outUpdateClients << list[i].phone << '\n';
	}
	outUpdateClients.close();
	cout << "Дані клієнта успішно оновлено!\n";
}

void deleteClient() {
	ifstream readerFile(fileStorage);
	if (!readerFile.is_open()) {
		cout << "Файл не існує " << fileStorage << "\n";
		return;
	}

	Client marko;
	int count = 0;
	while (readerFile.getline(marko.name, 80, '\n')) {
		if (strlen(marko.name) == 0) break;
		readerFile.getline(marko.phone, 20, '\n');
		count++;
	}
	readerFile.close();

	if (count == 0) {
		cout << "Список клієнтів порожній!\n";
		return;
	}
	int numberClient;
	cout << "Вкажіть номер клієнта, якого потрібно видалити\n";
	cout << "->_";
	cin >> numberClient;

	if (numberClient <= 0 || numberClient > count) {
		cout << "Номер клієнта вказано невірно!\n";
		return;
	}

	// Переносимо дані у тимчасовий файл, ігноруючи клієнта, що видаляється
	readerFile.open(fileStorage);
	ofstream tempFile("temp.txt");

	int currentClient = 0;
	while (readerFile.getline(marko.name, 80, '\n')) {
		if (strlen(marko.name) == 0) break;
		readerFile.getline(marko.phone, 20, '\n');
		currentClient++;

		// Якщо це не той клієнт, якого видаляємо, записуємо у новий файл
		if (currentClient != numberClient) {
			tempFile << marko.name << "\n";
			tempFile << marko.phone << "\n";
		}
	}
	readerFile.close();
	tempFile.close();

	// Видаляємо старий файл і перейменовуємо тимчасовий
	remove(fileStorage);
	rename("temp.txt", fileStorage);

	cout << "Клієнта успішно видалено!\n";
}

void searchClientByPhone() {
	ifstream readerFile(fileStorage);
	if (!readerFile.is_open()) {
		cout << "Файл не існує " << fileStorage << "\n";
		return;
	}

	char searchPattern[20];
	cout << "Введіть номер телефону (або його частину) для пошуку:\n";
	cout << "->_";
	cin.getline(searchPattern, 20, '\n');

	Client marko;
	int foundCount = 0;
	int totalCount = 0;

	cout << "\n--- Результати пошуку ---\n";
	while (readerFile.getline(marko.name, 80, '\n')) {
		if (strlen(marko.name) == 0) continue;
		readerFile.getline(marko.phone, 20, '\n');
		totalCount++;

		// Перевіряємо чи є шуканий рядок підрядком у номері телефону клієнта
		if (strstr(marko.phone, searchPattern) != nullptr) {
			foundCount++;
			cout << "Позиція у списку: " << totalCount << "\n";
			cout << "Name: " << marko.name << "\n";
			cout << "Phone: " << marko.phone << "\n";
			cout << "-------------------------\n";
		}
	}
	readerFile.close();

	if (foundCount == 0) {
		cout << "Клієнтів із таким номером телефону не знайдено.\n";
	}
	else {
		cout << "Знайдено клієнтів: " << foundCount << "\n";
	}
}