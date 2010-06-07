
typedef int* scons_check_type;

int main()
{
    static int test_array[1 - 2 * !(((long int) (sizeof(scons_check_type))) <= 7)];
    test_array[0] = 0;

    return 0;
}
