#include <ctype.h>
#include <errno.h>
#include <limits.h>

int check_base(int base, char* str, int* id);
int check_sign(int sign, char* str, int* id);
long overflow_occured(int is_overflowed, int sign, long* number);
long calculate_number(int sign, int base, long* number, int* id, int* is_overflowed, long curr_number);
long get_curr_number(int* id, long* curr_number, char* str);

long strtol(const char* nPtr, char** endPtr, int base) {
    char* str = (char*) nPtr;
    int sign = 1; // 0 - negative value  1 - positive value
    long curr_number = 0;
    long number = 0;
    int id = 0;
    int is_overflowed = 0;
    
    if (endPtr)
		*endPtr = (char*)nPtr;

    while (isspace(str[id])) {
        id++;
    }

    sign = check_sign(sign, str, &id);

    base = check_base(base, str, &id);

    if (base == 16) {
        if (str[id] == '\0') {
            *endPtr = &str[id - 1];
            return number;
        }
        else if (islower(str[id])) {
            if (str[id] > 'f') {
                *endPtr = &str[id - 1];
                return number;
            }
        }
        else if (isupper(str[id])) {
            if (str[id] > 'F') {
                *endPtr = &str[id - 1];
                return number;
            }
        }
    }

    int while_executed = 0;
    do {
        curr_number = get_curr_number(&id, &curr_number, str);
        
        if (curr_number < 0)
            break;

        if (curr_number >= base)
            break;

        number = calculate_number(sign, base, &number, &id, &is_overflowed, curr_number);

        while_executed = 1;
    } while (str[id] != '\0');

    number = overflow_occured(is_overflowed, sign, &number);

    if (endPtr != 0) {
        if (is_overflowed == 1 || while_executed == 1)
            *endPtr = &str[id];
        else 
            *endPtr = (char*)nPtr;
    }

    return number;
}

int check_sign(int sign, char* str, int* id) {
    if (str[*id] == '-') {
        sign = 0;
        *id += 1;
    }
    else if (str[*id] == '+') {
        sign = 1;
        *id += 1;
    }
    return sign;
}

int check_base(int base, char* str, int* id) {
        if (base == 0) {
        if (str[*id] == '0') {
            if (str[*id + 1] == 'x' || str[*id + 1] == 'X') {
                base = 16;
                *id += 2;
            }
            else {
                base = 8;
            }
        }
        else {
            base = 10;
        }
    }
    if (base == 16) {
        if (str[*id] == '0' && (str[*id + 1] == 'x' || str[*id + 1] == 'X')) {
            *id += 2;
        }
    }
    if (base > 36 || base < 2) {
        errno = EINVAL;
        return 0;
    }

    return base;
}

long overflow_occured(int is_overflowed, int sign, long* number) {
    if (is_overflowed == 1) {
        if (sign == 1)
            *number = LONG_MAX;
        else
            *number = LONG_MIN;
        errno = ERANGE;
    }
    return *number;
}

long calculate_number(int sign, int base, long* number, int* id, int* is_overflowed, long curr_number) {
    if (sign == 1) {
        if (*is_overflowed == 1 || *number > (LONG_MAX - curr_number) / base) {
            *id += 1;
            *is_overflowed = 1;
        }
        else {
            *number = *number * base + curr_number;
            *id += 1;
        }
    }
    else {
        if (*is_overflowed == 1 || *number < (LONG_MIN + curr_number) / base) {
            *id += 1;
            *is_overflowed = 1;
        }
        else {
            *number = *number * base - curr_number;
            *id += 1;
        }
    }

    return *number;
}

long get_curr_number(int* id, long* curr_number, char* str) {
    if (isdigit(str[*id])) {
        *curr_number = str[*id] - '0';
        return *curr_number;
    }
    else if (isalpha(str[*id])) {
        if (islower(str[*id])) {
            *curr_number = (str[*id] - 'a') + 10;
            return *curr_number;
        }
        else {
            *curr_number = (str[*id] - 'A') + 10;
            return *curr_number;
        }
    }
    else
        return -1;
}
