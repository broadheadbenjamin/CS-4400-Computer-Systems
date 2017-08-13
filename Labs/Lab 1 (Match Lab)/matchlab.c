#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


int main(int argC, char* argV[]){
    // Create method variables
    char mode = 'a';      // Default mode is 'a'.
    int translate = 0;  // 0 = don't translate, 1 = do translate.
    char* input_string; // The input string

    // Loop through the input arguments
    int i;
    for (i = 1; i < argC; i++){
        if ( argV[i][0] == '-'){                                // Check for flags
            char temp = argV[i][1];                             // Set temp to whatever is after the dash
            if (temp == 'a' || temp == 'b' || temp == 'c'){     // Check if you are in mode a, b, or c.
                mode = temp;
            } else if (temp == 't'){                            // See if the translate mode is enabled.
                translate = 1;
            }
        }else{
            input_string = strdup(argV[i]);                      // Store the input string into a variable.
        }
    }

    // Execute the appropriate mode
    if (mode == 'a'){
        int v_count = 0;
        int decimal_count = 0;
        int length = strlen(input_string);

        for (i = 0; i < length; i++){
            if (isdigit(input_string[i]))  //See if the current character is a decimal.
                decimal_count++;
            else if (input_string[i] == 'v')                    // See if the current character is a 'v'.
                v_count++;
        }

        if (translate == 1){
            if (v_count >= 4 && v_count <=7 && decimal_count >= 1 && decimal_count <= 3){
                for (i = 0; i < length; i++){
                    printf("%c", input_string[length - i]);
                }
            }
        }else{
            if (v_count >= 4 && v_count <=7 && decimal_count >= 1 && decimal_count <= 3)
                printf("Yes ");
            else
                printf("No ");
        }
    }else if (mode == 'b'){
        int length = strlen(input_string);
        int decimal_count = 0;
        bool x_odd = false;
        int capital_count = 0;
        char X[4];
        bool is_match = false;
        bool odd_capitals = false;
        bool m_even = true;

        for (i = 0; i < length; i++){
            if (isdigit(input_string[i])){                      //See if the current character is a decimal.
                X[decimal_count] = input_string[i];
                decimal_count++;
            }else if (input_string[i] == 'x'){                  // See if the current character is an x
                x_odd = ~x_odd;
            }
            else if (input_string[i] == 'm'){                   // See if the current character is an m
                m_even = ~m_even;
            } else if (isupper(input_string[i])){               // See if the character is a capital letter.
                capital_count++;
                odd_capitals = ~odd_capitals;
            }
        }

        if (m_even == true && decimal_count <= 4 && x_odd == true && odd_capitals == true){ // See if match conditions are met.
            if (decimal_count == 3){
                if (X[1] == X[2])
                    is_match = true;
            }
            if (decimal_count == 4){
                if (X[1] == X[3])
                    is_match = true;
            }
        }

        if (is_match == true){
            if (translate == 1){
                for (i = 0; i < length; i++){
                    printf("%c", input_string[i]);
                    int temp = i & 7;
                    printf("%d", temp);
                }
            }else{
                printf("Yes ");
            }
        } else{
            printf("No ");
        }
    }else if (mode == 'c'){
        bool k_even = true;
        char X[256];
        bool add_to_X = true;
        bool p_odd = false;
        char X_rev[256];
        int digit_count = 0;
        int length = strlen(input_string);
        int x_i = 0;
        int x_rev_i = 0;
        bool odd_upper = false;
        bool rev_match = true;

        for (i = 0; i < length; i++){
            if (input_string[i] == 'k'){
                k_even = ~k_even;
            }else if (isupper(input_string[i]) == true){
                if (add_to_X == true){
                    X[x_i] = input_string[i];
                    x_i++;
                    odd_upper = ~odd_upper;
                    if (isupper(input_string[i+1]) == false){
                        add_to_X = false;
                        }
                } else if (add_to_X == false){
                    X_rev[256 - x_rev_i] = input_string[i];
                    x_rev_i++;
                }
            }else if(input_string[i] == 'p'){
                p_odd = ~p_odd;
            }else if(isdigit(input_string[i])){
                digit_count++;
            }
        }


        if (k_even == true && odd_upper == true && digit_count <= 3 && digit_count >= 1 && p_odd == true){
            if (translate == 1){
                for (i = 0; i < length; i++){
                        printf("%c", input_string[i]);
                    if (input_string[i] == 'H'){
                        printf("%c",'H');
                    }
                }
            }else{
                printf("Yes ");
            }
        }else{
            printf("No ");
        }
    }
}

