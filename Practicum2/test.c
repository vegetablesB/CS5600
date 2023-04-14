#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Add export PATH=$PATH:/home/ning/Workspace/HW12/Practicum2 to ~/.bashrc for use of fget command

void run_test(const char *test_name, const char *command)
{
    printf("Running test case: %s\n", test_name);
    printf("Command: %s\n", command);

    int result = system(command);

    if (result == 0)
    {
        printf("Test case PASSED\n\n");
    }
    else
    {
        printf("Test case FAILED with error code: %d\n\n", result);
    }
}

int main()
{
    run_test("1. GET with local file path", "fget GET server/server.txt /home/ning/Workspace/HW12/local.txt");
    run_test("2. GET with missing local file path", "fget GET server/server.txt");

    run_test("3. INFO command", "fget INFO server/server.txt");

    run_test("4. MD for a folder", "fget MD server/MD_folder");

    run_test("5. PUT with remote file path", "fget PUT /home/ning/Workspace/HW12/clientfile.txt server/serverfile.txt");
    run_test("6. PUT with missing remote file path", "fget PUT /home/ning/Workspace/HW12/clientfile.txt");

    run_test("pre. PUT with remote file path", "fget PUT /home/ning/Workspace/HW12/clientfile.txt server/RM_file.txt");
    run_test("pre. RM for a folder", "fget MD server/RM_folder");

    run_test("7. RM for a file", "fget RM server/RM_file.txt");
    run_test("8. RM for a folder", "fget RM server/RM_folder");

    return 0;
}
