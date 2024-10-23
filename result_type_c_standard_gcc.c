#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure Result
typedef struct {
    int type;
    union {
        void* value;
        const char* error;
    };
} Result;

// Define the constants OK and ERR
#define OK 0
#define ERR 1

// Define the macros OK_RESULT and ERR_RESULT
#define OK_RESULT(val) (Result){ .type = OK, .value = (void*)(val) }
#define ERR_RESULT(msg) (Result){ .type = ERR, .error = msg }

// Cleaning function to release memory assigned to the structure pointer results
void cleanup_result(Result* res) {
    if (res->type == OK && res->value != NULL) {
        free(res->value);
    }
}

// Cleaning function to release memory assigned to the structure pointer results
void cleanup(void* ptr) {
    if (*(void**)ptr != NULL) {
        free(*(void**)ptr);
    }
}

// Define the macro CLEANUP_RESULT
#define RESULT_AUTO_CLEANED(value) __attribute__((cleanup(cleanup_result))) Result value

// Function to read the user's input safely
Result getline_stdin() {
    size_t size = 128; // Initial buffer size
    size_t pos = 0;    // Current position in the buffer
    char *buffer = malloc(size);
    if (buffer == NULL) {
        return ERR_RESULT("Memory allocation error");
    }
    int c;
    while ((c = fgetc(stdin)) != EOF && c != '\n') {
        buffer[pos++] = (char)c;
        // If we reach the size of the buffer, we expand it
        if (pos >= size) {
            size *= 2;
            char *new_buffer = realloc(buffer, size);
            if (new_buffer == NULL) {
                free(buffer);
                return ERR_RESULT("Error de reasignación de memoria");
            }
            buffer = new_buffer;
        }
    }
    // Handle the case of EOF without entry
    if (pos == 0 && c == EOF) {
        free(buffer);
        return ERR_RESULT("Error al leer la entrada");
    }
    buffer[pos] = '\0'; // Finish the chain with '\ 0'
    return OK_RESULT(buffer);
}

// Function to say hello to the user
Result say_hello(const char* name) {
    size_t len = strlen("hello ") + strlen(name) + strlen(" ! ") + 1;
    char* greeting = malloc(len);
    if (greeting == NULL) {
        return ERR_RESULT("Memory allocation error");
    }
    snprintf(greeting, 50, "Hello, %s!", name);
    return OK_RESULT(greeting);
}

// Function to ask the user a question and read the answer safely
Result answer_user_input(const char* question) {
    printf("%s", question);
    return getline_stdin();
}

// Function to say hello to the developer
Result say_hello_to_dev() {
    RESULT_AUTO_CLEANED(name) = answer_user_input("What is your name, bro? it's just a test: "); 
    switch (name.type) {
        case OK:
            return say_hello((char*)name.value);
            break;
        case ERR:
            printf("Error: %s\n", name.error);
            break;
    }
    return ERR_RESULT(name.error);
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


int main() {
    
    //  Variable to store the result value
    int resultValue = 0;
    // Use of the result value outside the IF and Switch
    RESULT_AUTO_CLEANED(result) = secure_division(10, 2);
    // handle the result with an IF
    if (result.type == OK) {
        int value = *(int*)result.value;
        resultValue = value;
        printf("Result: %d\n", value);
    } else {
        printf("Error: %s\n", result.error);
    }

    // handle the result with a Switch
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

    // Variable to store the result value
    char* result2Value = NULL;
    // Use of the result value outside the IF and Switch
    RESULT_AUTO_CLEANED(result2) = say_hello("Manuel");

    // handle the result with an IF
    if (result2.type == OK) {
        char* greeting = (char*)result2.value;
        result2Value = greeting;
        printf("Result: %s\n", greeting);
    } else {
        printf("Error: %s\n", result2.error);
    }

    // handle the result with a Switch
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

    // Using a safe input function to get the user's name
    RESULT_AUTO_CLEANED(answer) = answer_user_input("What is your name? ");
    
    // handle the result with a Switch
    switch (answer.type) {
        case OK: {
            char* name = (char*)answer.value;
            printf("Your name is: %s\n", name);
            break;
        }
        case ERR: {
            printf("Error: %s\n", answer.error);
            break;
        }

    }

    RESULT_AUTO_CLEANED(hello_dev) = say_hello_to_dev();

    switch (hello_dev.type) {
        case OK: {
            char* greeting = (char*)hello_dev.value;
            printf("Result: %s\n", greeting);
            break;
        }
        case ERR: {
            printf("Error: %s\n", hello_dev.error);
            break;
        }
    }



    /*

        The use of the macro RESULT_AUTO_CLEANED is proposed to facilitate,   
        when leaving the Main function, the variables declared with 
        the macro RESULT_AUTO_CLEANED are automatically cleaned.
        Therefore, it is not necessary to call the Cleanup_Result
        to release the memory assigned to the leaders of the result.
        
        The same behavior would happen if variables are declared within 
        other functions,that is, when leaving the function, the variables 
        declared with the Macro RESULT_AUTO_CLEANED, they are cleaned 
        automatically.

        In addition, a method to handle inputs insurance user is proposed
        using the function getline_stdin, which is responsible for reading the
        User entry safely and handle memory errors
        that may arise during the execution of the function.

    */

    return 0;
}