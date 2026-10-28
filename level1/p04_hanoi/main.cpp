#include <stdio.h>
void hanoi_move(int disk_count, char from_rod, char auxiliary_rod, char target_rod)
{
    if (disk_count <= 0)
        return;
    if (disk_count == 1)
    {
        printf("%c -> %c\n", from_rod, target_rod);
        return;
    }
    hanoi_move(disk_count - 1, from_rod, target_rod, auxiliary_rod);
    printf("%c -> %c\n", from_rod, target_rod);
    hanoi_move(disk_count - 1, auxiliary_rod, from_rod, target_rod);
}
int main()
{
    const int total_disks = 3;
    hanoi_move(total_disks, 'A', 'B', 'C');
    return 0;
}