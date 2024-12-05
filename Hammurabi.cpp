#include <iostream>
#include <fstream>
#include <cstdlib> // Для генерации случайных чисел
#include <ctime>   // Для инициализации генератора случайных чисел

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");

    // Инициализация параметров игры
    int population = 100; // Население
    int acres = 1000;     // Земля
    int wheat = 2800;     // Пшеница
    int year = 1;         // Текущий год
    int dead = 0;         // Умерло жителей
    int arrivals = 0;     // Приехало людей
    int rats_percent = 0; // Бушелей съели крысы
    bool plague = false;  // Есть ли чума

    srand(time(0)); // Инициализация генератора случайных чисел

    // Проверка наличия сохраненной игры
    ifstream saveFile("savegame.txt");
    if (saveFile.is_open()) {
        char load_choice;
        cout << "Найдена сохраненная игра. Хотите продолжить? (y/n): ";
        cin >> load_choice;

        if (load_choice == 'y' || load_choice == 'Y') {
            saveFile >> year >> population >> acres >> wheat >> dead >> arrivals >> plague;
            saveFile.close();
            cout << "Сохраненная игра успешно загружена!\n";
        }
        else {
            saveFile.close();
            remove("savegame.txt");
            cout << "Начинаем новую игру.\n";
        }
    }

    // Основной цикл игры
    while (year <= 10 && population > 0) {

        // Предложение сохранить игру
        char save_choice;
        cout << "Хотите сохранить игру и выйти? (y/n): ";
        cin >> save_choice;

        if (save_choice == 'y' || save_choice == 'Y') {
            ofstream saveFile("savegame.txt");
            if (saveFile.is_open()) {
                saveFile << year << " " << population << " " << acres << " " << wheat << " " << dead << " " << arrivals << " " << plague << endl;
                saveFile.close();
                cout << "Игра сохранена. До скорой встречи, повелитель!\n";
                return 0; // Завершаем игру
            }
            else {
                cout << "Ошибка сохранения! Игра не может быть сохранена.\n";
            }
        }

        cout << "Мой повелитель, соизволь поведать тебе о текущем положении дел\n";

        // Вывод начальных параметров
        cout << "Год: " << year << "\n";
        cout << "Население: " << population << "\n";
        cout << "Акров земли: " << acres << "\n";
        cout << "Бушелей пшеницы: " << wheat << "\n";

        // Генерация случайной цены акра земли
        int land_price = 17 + rand() % 10; // Цена от 17 до 26
        cout << "Цена одного акра земли: " << land_price << " бушелей.\n";

        cout << "----------------------------------------------------------------" << endl;

        // Сколько земли купить
        int acres_to_buy;
        while (true) { // Цикл ввода до корректного значения
            cout << "Сколько акров земли вы хотите купить? ";
            cin >> acres_to_buy;

            int cost_to_buy = acres_to_buy * land_price;
            if (cost_to_buy <= wheat && acres_to_buy >= 0) {
                // Если ввод корректен, выходим из цикла
                acres += acres_to_buy;
                wheat -= cost_to_buy;
                cout << "Вы купили " << acres_to_buy << " акров земли.\n";
                break;
            }
            else {
                // Если ввод некорректен, показываем сообщение и повторяем
                cout << "Недостаточно пшеницы для покупки или введено некорректное значение. Попробуйте ещё раз.\n";
            }
        }

        cout << "----------------------------------------------------------------" << endl;

        // Сколько земли продать
        int acres_to_sell;
        while (true) {
            cout << "Сколько акров земли вы хотите продать? ";
            cin >> acres_to_sell;

            // Проверка, достаточно ли земли
            int cost_to_sell = acres_to_sell * land_price;
            if (acres_to_sell <= acres && acres_to_sell >= 0) {
                acres -= acres_to_sell;
                wheat += cost_to_sell;
                cout << "У вас " << acres << " акров земли и " << wheat << " бушелей пшеницы" << endl;
                break;
            }
            else {
                cout << "У вас недостаточно земли для продажи " << acres_to_sell << " акров" << endl;
            }
        }
        cout << "----------------------------------------------------------------" << endl;

        // Пшеница для еды
        int wheat_for_food;
        while (true) {
            cout << "Сколько бушелей пшеницы повелеваешь съесть? " << endl;
            cin >> wheat_for_food;

            // Проверка, достаточно ли пшеницы
            if (wheat_for_food <= wheat && wheat_for_food >= 0) {
                wheat -= wheat_for_food;
                cout << "Вы потратили " << wheat_for_food << " бушелей пшеницы на еду" << endl;
                break;
            }
            else {
                cout << "Пшеницы недостаточно, ваше величество" << endl;
                wheat_for_food = 0;
            }
        }

        cout << "----------------------------------------------------------------" << endl;

        // Засев земли
        int acres_to_sow;
        while (true) { // Цикл ввода до корректного значения
            cout << "Сколько акров земли вы хотите засеять? ";
            cin >> acres_to_sow;

            int wheat_needed_for_seeds = acres_to_sow * 0.5;
            int max_acres_by_population = population * 10;

            if (acres_to_sow <= acres && wheat_needed_for_seeds <= wheat && acres_to_sow <= max_acres_by_population && acres_to_sow >= 0) {
                // Если ввод корректен, выходим из цикла
                wheat -= wheat_needed_for_seeds;
                cout << "Вы засеяли " << acres_to_sow << " акров земли.\n";
                break;
            }
            else {
                // Если ввод некорректен, показываем сообщение и повторяем
                cout << "Недостаточно ресурсов для засева или введено некорректное значение. Попробуйте ещё раз.\n";
            }
        }


        cout << "----------------------------------------------------------------" << endl;

        // Сбор урожая
        int profit_per_acre = 1 + rand() % 6;
        int harvested_wheat = profit_per_acre * acres_to_sow;
        wheat += harvested_wheat;
        cout << "Вы собрали " << harvested_wheat << " бушелей пшеницы, по " << profit_per_acre << " бушелей с акра" << endl;
        cout << "Сейчас у вас " << wheat << " бушелей пшеницы" << endl;

        cout << "----------------------------------------------------------------" << endl;

        // Крысы
        rats_percent = rand() % 8;
        int wheat_eaten_by_rats = (rats_percent * wheat) / 100;
        wheat -= wheat_eaten_by_rats;
        cout << "Крысы съели " << wheat_eaten_by_rats << " бушелей пшеницы" << endl;
        cout << "Сейчас у вас " << wheat << " бушелей пшеницы" << endl;

        cout << "----------------------------------------------------------------" << endl;

        // Голод
        int wheat_needed_for_population = population * 20;
        int starved = 0;

        if (wheat_for_food < wheat_needed_for_population) {
            int deficit = wheat_needed_for_population - wheat_for_food;
            starved = deficit / 20;
            if (starved > population) {
                starved = population; // Смертей не может быть больше, чем жителей
            }
            population -= starved;
            cout << starved << " человек умерли с голоду" << endl;
        }
        else {
            cout << "Все жители сыты" << endl;
        }

        cout << "----------------------------------------------------------------" << endl;

        // Новые жители
        arrivals = (starved / 2) + (5 - profit_per_acre) * (wheat / 600) + 1;
        if (arrivals < 0) arrivals = 0;
        if (arrivals > 50) arrivals = 50;
        population += arrivals;
        cout << arrivals << " человек прибыли в ваш город" << endl;

        cout << "----------------------------------------------------------------" << endl;

        // Проверка массовой смерти от голода
        double starvation_rate = (static_cast<double>(starved) / (population + starved)) * 100;
        if (starvation_rate > 45) {
            cout << "За этот год от голода умерло более 45% населения!" << endl;
            cout << "Народ устроил бунт, и ваше правление закончилось!" << endl;
            return 0; // Выход из цикла при поражении
        }

        // Чума
        if (rand() % 100 < 15) {
            plague = true;
            population /= 2;
            cout << "Чума убила половину населения, в городе осталось людей: " << population << endl;
        }
        else {
            plague = false;
        }

        // Итоги текущего раунда
        cout << "Итоги года: " << endl;
        cout << "Население: " << population << endl;
        cout << "Бушелей пшеницы в закромах: " << wheat << endl;
        cout << "Акров земли: " << acres << endl;
        cout << endl;
        cout << "----------------------------------------------------------------" << endl;
        cout << "----------------------------------------------------------------" << endl;
        cout << endl;

        // Переход к следующему году
        year++;
    }

    // Игра завершена
    cout << "Игра окончена.\n";

    // Подсчет среднегодового процента умерших от голода
    int total_starved = dead; // Общий счетчик умерших за все годы
    double average_starved_percent = (total_starved * 100.0) / (100 * year); // В начале было 100 человек

    // Подсчет количества акров на одного жителя
    double acres_per_person = static_cast<double>(acres) / population;

    // Вывод итогов
    cout << "Среднегодовой процент умерших от голода: " << average_starved_percent << "%" << endl;
    cout << "Акров на одного жителя: " << acres_per_person << endl;

    // Оценка результатов
    if (average_starved_percent > 33 || acres_per_person < 7)
    {
        cout << "Из-за вашей некомпетентности в управлении, народ устроил бунт, и изгнал вас из города " << endl;
    }
    else if (average_starved_percent > 10 || acres_per_person < 9)
    {
        cout << "Вы правили железной рукой. Народ вздохнул с облегчением, когда ваше правление закончилось" << endl;
    }
    else if (average_starved_percent > 3 || acres_per_person < 10)
    {
        cout << "Вы справились неплохо. Многие хотели бы видеть вас правителем снова" << endl;
    }
    else
    {
        cout << "Фантастика! Вы стали легендарным правителем, которым будут восхищаться веками" << endl;
    }



    return 0;
    }


