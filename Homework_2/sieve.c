#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

//First Thread, exists to print out the prime values
void *myThreadFun(void *vargp){
    int *n = (int*) vargp;
    int number = *n;
    int j,i;
    int primes[number+1];
    printf("PRIME Thread\n");
    //populate an array with integers up to the inputted number
    for(i = 2; i<=number; i++)
        primes[i] = i;
//While loop to test if a number is prime and set all composite numbers equal to zero
    i = 2;
    while ((i*i) <= number)
    {
        if (primes[i] != 0)
        {
            for(j=2; j<number; j++)
            {
                if (primes[i]*j > number)
                    break;
                else
                    //makes composite elements 0
                    primes[primes[i]*j]=0;
            }
        }
        i++;
    }
//prints the prime numbers
    for(i = 2; i<=number; i++)
    {
        //prints all nonzero array elements
        if (primes[i]!=0)
            printf("%d\n",primes[i]);

    }
    printf("\n\n");
    return NULL; 
}

//second thread prints the subset of the first array that have prime swaps
void *inPrint(void *vargp){
    int *n = (int*) vargp;
    int number = *n;
    int j,i;
    int primes[number+1];
    int swap[number+1];
    printf("Inverse Thread\n");
//generates the prime array the same way as before
    for(i = 2; i<=number; i++)
        primes[i] = i;
    i = 2;
    while ((i*i) <= number)
    {
        if (primes[i] != 0)
        {
            for(j=2; j<number; j++)
            {
                if (primes[i]*j > number)
                    break;
                else
                    //makes composite elements 0
                    primes[primes[i]*j]=0;
            }
        }
        i++;
    }
    //swaps the digits of all the two didgit prime numbers
    for(i = 2; i<=number; i++)
    {
        swap[i]=primes[i];
        if (primes[i]>=10){
            int twoD = swap[i];
            int tens = (twoD/10)%10; 
            int ones = twoD%10;
            twoD = (ones*10)+tens;
            swap[i] = twoD;
        }
    }
    //new loop to test for composite numbers and set their values to zero
    i = 2;
    while(i<number){
        for(j=2; j<=swap[i]/2; ++j)
        {
            // condition for nonprime number
            if(swap[i]%j==0)
            {
                swap[i]=0;
                break;
            }
        }   
    i++;     
    }
    //clears the values in the prime array so it can be reused
    memset(primes, 0, number*sizeof(int));
    //swap the numbers back again
    for(i = 2; i<=number; i++)
    {
        primes[i]=swap[i];
        if (swap[i]>=10){
            int twoD = primes[i];
            int tens = (twoD/10)%10; 
            int ones = twoD%10;
            twoD = (ones*10)+tens;
            primes[i] = twoD;
        }
    }
    //if the number is not 0 and greater than ten we print it as part of this subset
    for(i = 2; i<=number; i++)
    {
        if (primes[i]!=0 && primes[i]>=10)
            printf("%d\n",primes[i]);
    }
    printf("\n\n");
    return NULL; 
}

int main()
{
    printf("Enter the number\n");
    int i,j;
    int* number;
    scanf("%d",number);
    printf("\n");
    pthread_t tid,tid2;//two thread ids
    pthread_create(&tid, NULL, myThreadFun, (void*)number); //main thread
    pthread_create(&tid2, NULL, inPrint, (void*)number); //secondary thread
    pthread_join(tid, NULL); //thread 1 finishes
    pthread_join(tid2, NULL);//thread two finishes

    return 0;
}