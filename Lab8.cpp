.global Bills

Bills:
    PUSH    {R4-R6}          // Preserve registers
    MOV     R4, SP           // Save current stack pointer
    SUB     SP, SP, 16       // Allocate space for BILLS struct

    // twenties
    MOV     R1, RDI          // Copy dollars to R1
    MOV     R2, 20           // Load 20 into R2
    MOV     R3, 0            // Initialize counter for twenties
    MOV     R5, 0            // Initialize quotient
    CMP     R1, R2           // Compare R1 with 20
    CMOVGE  R5, R5, 1        // Increment quotient if R1 >= 20
    SUB     R1, R1, R2       // Subtract 20 from R1
    CMP     R1, R2           // Compare R1 with 20
    CMOVGE  R5, R5, 1        // Increment quotient if R1 >= 20
    STR     R5, [SP]         // twenties in R5

    // tens
    MOV     R2, 10           // Load 10 into R2
    MOV     R3, 0            // Initialize counter for tens
    MOV     R5, 0            // Initialize quotient
    CMP     R1, R2           // Compare R1 with 10
    CMOVGE  R5, R5, 1        //  Increment quotient if R1 >= 10
    SUB     R1, R1, R2       // Subtract 10 from R1
    CMP     R1, R2           // Compare R1 with 10
    CMOVGE  R5, R5, 1        // Increment quotient if R1 >= 10
    STR     R5, [SP, 4]      // tens in R5

    // fives
    MOV     R2, 5            // Load 5 into R2
    MOV     R3, 0            //  Initialize counter for fives
    MOV     R5, 0            // Initialize quotient
    CMP     R1, R2           // Compare R1 with 5
    CMOVGE  R5, R5, 1        // Increment quotient if R1 >= 5
    SUB     R1, R1, R2       //Subtract 5 from R1
    CMP     R1, R2           // Compare R1 with 5
    CMOVGE  R5, R5, 1        // Increment quotient if R1 >= 5
    STR     R5, [SP, 8]      // fives in R5

   // ones 
    STR     R1, [SP, 12]     // ones in R5
    MOV     SP, R4           // Restore stack pointer
    POP     {R4-R6}          // Restore registers
    BX      LR               // Return


.global Coins

Coins:
    PUSH    {R4-R6}          // Preserve registers
    MOV     R4, SP           // Save current stack pointer
    SUB     SP, SP, 16       // Allocate space for COINS struct

    // quarters
    MOV     R1, RDI          // Copy cents to R1
    MOV     R2, 25           // Load 25 into R2
    MOV     R3, 0            // Initialize counter for quarters
    MOV     R5, 0            // Initialize quotient
    CMP     R1, R2           // Compare R1 with 25
    CMOVGE  R5, R5, 1        // Increment quotient if R1 >= 25
    SUB     R1, R1, R2       // Subtract 25 from R1
    CMP     R1, R2           // Compare R1 with 25
    CMOVGE  R5, R5, 1        // Increment quotient if R1 >= 25
    STR     R5, [SP]         // Store the result in coins->quarters

    // dimes
    MOV     R2, 10           // Load 10 into R2
    MOV     R3, 0            // Initialize counter for dimes
    MOV     R5, 0            // Initialize quotient
    CMP     R1, R2           // Compare R1 with 10
    CMOVGE  R5, R5, 1        // Increment quotient if R1 >= 10
    SUB     R1, R1, R2       // Subtract 10 from R1
    CMP     R1, R2           // Compare R1 with 10
    CMOVGE  R5, R5, 1        // Increment quotient if R1 >= 10
    STR     R5, [SP, 4]      // Store the result in coins->dimes

    // nickels
    MOV     R2, 5            // Load 5 into R2
    MOV     R3, 0            // Initialize counter for nickels
    MOV     R5, 0            // Initialize quotient
    CMP     R1, R2           // Compare R1 with 5
    CMOVGE  R5, R5, 1        // Increment quotient if R1 >= 5
    SUB     R1, R1, R2       // Subtract 5 from R1
    CMP     R1, R2           // Compare R1 with 5
    CMOVGE  R5, R5, 1        // Increment quotient if R1 >= 5
    STR     R5, [SP, 8]      // Store the result nickels in R5

    STR     R1, [SP, 12]     // Store the remaining cents in coins->pennies

    MOV     SP, R4           // Restore stack pointer
    POP     {R4-R6}          // Restore registers
    BX      LR               // Return
