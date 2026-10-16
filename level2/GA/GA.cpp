#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

constexpr int POPULATION_SIZE = 2000;
constexpr int GENE_LENGTH = 100;
constexpr int ROWS = 20;
constexpr int COLS = 20;
constexpr int GENERATIONS = 2000;

static constexpr int direction_x[4] = {1, 0, -1, 0},direction_y[4] = {0, 1, 0, -1};

static mt19937 random_engine(time(nullptr));

namespace {
    struct Individual
    {
        int chromosome[GENE_LENGTH];
        int x, y, fitness;
    };
}

static Individual population[POPULATION_SIZE];
static Individual next_population[POPULATION_SIZE];

static char maze[ROWS][COLS + 1] =
    {
        "####################",
        "#@.................#",
        "###....###########.#",
        "#..................#",
        "#.######....########",
        "#..................#",
        "#####....#####.#",
        "#..................#",
        "#.########....######",
        "#..................#",
        "###.....##########.#",
        "#..................#",
        "#.##########....####",
        "#..................#",
        "######....########.#",
        "#..................#",
        "#.###########...####",
        "#..................#",
        "##################.$",
        "####################"};
static void print_path(int index)
{
    int current_x = 0;
    int current_y = 1;

    for (int i = 0; i < GENE_LENGTH; ++i)
    {
        int next_x = current_x + direction_x[population[index].chromosome[i]];
        int next_y = current_y + direction_y[population[index].chromosome[i]];

        if (next_x < 0 || next_x >= ROWS || next_y < 0 || next_y >= COLS || maze[next_x][next_y] == '#')
            continue;

        current_x = next_x;
        current_y = next_y;
        maze[current_x][current_y] = '*';

        if (population[index].chromosome[i] == 0)
            cout << "下 ";
        else if (population[index].chromosome[i] == 1)
            cout << "右 ";
        else if (population[index].chromosome[i] == 2)
            cout << "上 ";
        else if (population[index].chromosome[i] == 3)
            cout << "左 ";

        if (current_x == 18 && current_y == 19)
        {
            cout << "到达终点" << endl;
            for (int row = 0; row < ROWS; ++row)
            {
                for (int col = 0; col < COLS; ++col)
                {
                    cout << maze[row][col];
                }
                cout << endl;
            }
            return;
        }
    }
    cout << endl;
}

static int manhattan_distance(int x, int y)
{
    return abs(x - 18) + abs(y - 19);
}

static int calculate_fitness(Individual &individual)
{
    individual.x = 0;
    individual.y = 1;

    int fitness = 0;

    for (int i = 0; i < GENE_LENGTH; ++i)
    {
        int next_x = individual.x + direction_x[individual.chromosome[i]];
        int next_y = individual.y + direction_y[individual.chromosome[i]];

        if (next_x < 0 || next_x >= ROWS || next_y < 0 || next_y >= COLS || maze[next_x][next_y] == '#')
        {
            fitness -= 100;
            continue;
        }

        int old_distance = manhattan_distance(individual.x, individual.y);

        individual.x = next_x;
        individual.y = next_y;

        if (maze[next_x][next_y] == '$')
        {
            return 1000000 + (GENE_LENGTH - i) * 1000;
        }

        fitness += old_distance - manhattan_distance(individual.x, individual.y);
    }

    fitness -= manhattan_distance(individual.x, individual.y);

    return fitness;
}

static Individual &tournament_select()
{
    Individual *first = &population[random_engine() % POPULATION_SIZE];
    Individual *second = &population[random_engine() % POPULATION_SIZE];
    Individual *third = &population[random_engine() % POPULATION_SIZE];

    Individual *best = (first->fitness >= second->fitness) ? first : second;
    best = (best->fitness >= third->fitness) ? best : third;

    return *best;
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    for (int i = 0; i < POPULATION_SIZE; ++i)
    {
        int current_x = 0;
        int current_y = 1;

        for (int j = 0; j < GENE_LENGTH; ++j)
        {
            int valid_directions[4] = {0};
            int valid_count = 0;

            for (int k = 0; k < 4; ++k)
            {
                if (j > 0 && (population[i].chromosome[j - 1] + 2) % 4 == k)
                    continue;

                int next_x = current_x + direction_x[k];
                int next_y = current_y + direction_y[k];

                if (next_x >= 0 && next_x < ROWS && next_y >= 0 && next_y < COLS &&
                    (maze[next_x][next_y] == '.' || maze[next_x][next_y] == '@'))
                {
                    valid_directions[valid_count++] = k;
                }
            }

            if (valid_count == 0)
            {
                population[i].chromosome[j] = static_cast<int>(random_engine() % 4);
                continue;
            }

            int best_direction = valid_directions[0];

            for (int k = 1; k < valid_count; ++k)
            {
                int next_x = current_x + direction_x[valid_directions[k]];
                int next_y = current_y + direction_y[valid_directions[k]];

                int best_x = current_x + direction_x[best_direction];
                int best_y = current_y + direction_y[best_direction];

                if (manhattan_distance(next_x, next_y) < manhattan_distance(best_x, best_y))
                    best_direction = valid_directions[k];
            }

            population[i].chromosome[j] = (random_engine() % 100 < 80) ? best_direction : valid_directions[random_engine() % valid_count];

            current_x += direction_x[population[i].chromosome[j]];
            current_y += direction_y[population[i].chromosome[j]];
        }
    }

    int best_fitness = -1;
    Individual best_individual;

    for (int generation = 0; generation < GENERATIONS; ++generation)
    {
        for (int i = 0; i < POPULATION_SIZE; ++i)
            population[i].fitness = calculate_fitness(population[i]);

        int best_index = 0;

        for (int i = 1; i < POPULATION_SIZE; ++i)
        {
            if (population[i].fitness > population[best_index].fitness)
                best_index = i;
        }

        if (population[best_index].fitness > best_fitness)
        {
            best_fitness = population[best_index].fitness;
            best_individual = population[best_index];
        }

        next_population[0] = population[best_index];

        int mutation_rate = max(1, 5 - generation * 4 / GENERATIONS);

        for (int i = 1; i < POPULATION_SIZE; i += 2)
        {
            Individual &parent1 = tournament_select();
            Individual &parent2 = tournament_select();

            next_population[i] = parent1;

            if (i + 1 < POPULATION_SIZE)
                next_population[i + 1] = parent2;

            int crossover_point = static_cast<int>(random_engine() % GENE_LENGTH);

            if (i + 1 < POPULATION_SIZE)
            {
                for (int j = crossover_point; j < GENE_LENGTH; ++j)
                {
                    swap(next_population[i].chromosome[j],
                         next_population[i + 1].chromosome[j]);
                }
            }

            for (int j = 0; j < GENE_LENGTH; ++j)
            {
                if (random_engine() % 100 < mutation_rate)
                {
                    next_population[i].chromosome[j] = static_cast<int>(random_engine() % 4);
                }

                if (i + 1 < POPULATION_SIZE && random_engine() % 100 < mutation_rate)
                {
                    next_population[i + 1].chromosome[j] = static_cast<int>(random_engine() % 4);
                }
            }
        }

        swap(population, next_population);
    }

    if (best_fitness >= 1000000)
    {
        population[0] = best_individual;
        int steps = (best_fitness - 1000000) / 1000;
        cout << "找到路径，共 " << GENE_LENGTH - steps << " 步，路径为：";
        print_path(0);
    }
    else
    {
        printf("未找到路径\n");
    }

    return 0;
}
