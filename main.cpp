#include <stdio.h>
#include <stdlib.h>

int main(void){
    char msg1[] = "How many numbers: ";
    char msg2[] = "Enter a number: ";
    char outmsg1[] = "----------\nPositive: ";
    char outmsg2[] = "\nNegative: ";
    char outmsg3[] = "\nZero:     ";
    char digitStr[] = "%d";
    char padFiller[] = " ";
    int STACK_START, inVal, STACK_END, posnums = 0, negnums = 0, zernums = 0;

    _asm{
        jmp begin

    numLengthFrom10: //stores in ecx
        mov ecx, 0;
        s:
            add ecx, 1
            mov ebx, 10
            mov edx, 0
            div ebx
            add eax, 0
            jnz s
        mov eax, ecx
        mov ecx, 10
        sub ecx, eax
        ret
    
    printPadding: //prints padding ecx times
        faloop:
            push ecx
            lea eax, padFiller
            push eax
            call printf
            add esp, 4
            pop ecx
            loop faloop
        ret
    printResult:
        pop ebx //return address
        pop eax //outmsg
        push ebx //swap so return address can be kept safe, 
        push eax //whilst outmsg used for printf
        call printf
        add esp, 4 //ret address at top of stack, num second, outmsg no longer needed / available anyways

        pop ebx //return address
        pop eax //num
        push ebx
        push eax //saves num and keeps in eax
        call numLengthFrom10
        call printPadding //ret address at top of stack

        lea eax, digitStr //no need to push num (currently eax) as already push to correct position in stack
        push eax
        call printf
        add esp, 8 //ret address at top of stack
        ret

    begin:
        mov STACK_START, esp //REMOVE AT END (stack check)

        lea eax, msg1
        push eax
        call printf
        add esp, 4

        lea eax, inVal
        push eax
        lea eax, digitStr
        push eax
        call scanf
        add esp, 8
        mov ecx, inVal
            //stack is empty here, and ecx has counter in
    floop: 
        push ecx //loop counter pushed to stack

        lea edx, msg2
        push edx
        call printf
        add esp, 4

        lea eax, inVal
        push eax
        lea eax, digitStr
        push eax
        call scanf
        add esp, 8

        mov eax, inVal
        add eax, 0

        jnz eaxNotZero
            mov ebx, zernums
            add ebx, 1
            mov zernums, ebx
            jmp cleanup
    eaxNotZero:
            js eaxNegative
            mov ebx, posnums
            add ebx, 1
            mov posnums, ebx
            jmp cleanup
    eaxNegative:
            mov ebx, negnums
            add ebx, 1
            mov negnums, ebx
    cleanup:
        pop ecx 
        loop floop //stack is empty here
        
        push posnums
        lea eax, outmsg1
        push eax
        call printResult

        push negnums
        lea eax, outmsg2
        push eax
        call printResult

        push zernums
        lea eax, outmsg3
        push eax
        call printResult
    }

    return 0;
}