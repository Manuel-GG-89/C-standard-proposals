#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int type;
    union {
        void* value;
        const char* error;
    };
} Result;

#define OK 0
#define ERR 1
#define OK_RESULT(val) (Result){ .type = OK, .value = (void*)(val) }
#define ERR_RESULT(msg) (Result){ .type = ERR, .error = msg }

// Cleaning function to release memory assigned to the structure pointer results
void cleanup_result(Result* res) {
    if (res->type == OK && res->value != NULL) {
        free(res->value);
    }
}

// Define the macro CLEANUP_RESULT
#define RESULT_AUTO_CLEANED(value) __attribute__((cleanup(cleanup_result))) Result value

// Cleaning function to release memory assigned to the structure pointer results
void cleanup(void* ptr) {
    if (*(void**)ptr != NULL) {
        free(*(void**)ptr);
    }
}

Result secure_division(int numerator, int denominator) {
    if (denominator == 0) {
        return ERR_RESULT("Division by zero error");
    }
    int* result = malloc(sizeof(int));
    if (result == NULL) {
        return ERR_RESULT("Memory allocation error");
    }
    *result = numerator / denominator;
    return OK_RESULT(result);
}

Result say_hello(const char* name) {
    size_t len = strlen("hello ") + strlen(name) + strlen(" ! ") + 1;
    char* greeting = malloc(len);
    if (greeting == NULL) {
        return ERR_RESULT("Memory allocation error");
    }
    snprintf(greeting, 50, "Hello, %s!", name);
    return OK_RESULT(greeting);
}

int main() {

    int resultValue = 0;
    RESULT_AUTO_CLEANED(result) = secure_division(10, 2);
    if (result.type == OK) {
        int value = *(int*)result.value;
        resultValue = value;
        printf("Result: %d\n", value);
    } else {
        printf("Error: %s\n", result.error);
    }

    switch (result.type) {
        case OK: {
            int value = *(int*)result.value;
            resultValue = value;
            printf("Result: %d\n", value);
            break;
        }
        case ERR: {
            printf("Error: %s\n", result.error);
            break;
        }
    }

    // Use of the result value outside the IF and Switch
    if (resultValue != 0) {
        printf("Result: %d\n", resultValue);
    }

    char* result2Value = NULL;
    RESULT_AUTO_CLEANED(result2) = say_hello("Manuel");
    if (result2.type == OK) {
        char* greeting = (char*)result2.value;
        result2Value = greeting;
        printf("Result: %s\n", greeting);
    } else {
        printf("Error: %s\n", result2.error);
    }

    switch (result2.type) {
        case OK: {
            char* greeting = (char*)result2.value;
            result2Value = greeting;
            printf("Result: %s\n", greeting);
            break;
        }
        case ERR: {
            printf("Error: %s\n", result2.error);
            break;
        }
    }

    // Use of the result value outside the IF and Switch
    if (result2Value != NULL) {
        printf("Result: %s\n", result2Value);
    }

    /*
        When leaving the Main function, the variables
        declared with the Macro result_auto_Cleaned are automatically cleaned.
        Therefore, it is not necessary to call the Cleanup_Result
        to release the memory assigned to the leaders of the result.
        
        The same behavior would happen if variables are declared within other functions,
        That is, when leaving the function, the variables declared with the Macro result_Auto_Cleaned
        They are cleaned automatically.

    */
    return 0;
}