#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/**********************/
/**  define        */
/**********************/
typedef struct Cards {
    char* cardType;
    char* CardRank;
    int CardValue;
    struct Cards* next;
} Card;

typedef enum Results {
    blackjack,
    bust,
    youlose,
    youwin,
    dealerbust,
    tie,
    dealerwins

}result;
enum showSeconedCard{yes,no};

Card* deck=NULL;
Card* player=NULL;
Card* dealer=NULL;

/**********************/
/**  functions        */
/**********************/
char* GetCardType(int);			//get card type Hearts,Clubs,Diamonds,Spades
char* GetCardRank(int); 		//get card rank ace,2,3,4,5......King
int GetCardValue(int); 			//get card value 1 to 10
Card* Remove_card_deck(int);	//remove card by position
int get_cards_value(Card*);		//sum of cards value in lst of cards
int CountCardInDeck();			//counte card in deck
void show_cards(Card *,int);	//show cards in list of cards
void add_last_to_deck(Card*);	//add card to the end of deck
void add_last_to_player(Card*);	//add card to the end of player
void add_last_to_dealer(Card*); //add card to the end of dealer
void free_list(Card*);			//free memory from list of catds
int game(char*);					//start the game
void initcards(char*);				//init cards of the deck and give 2 cards to each player
void freememmory();				//free all cards

int main()
{
	srand(time(NULL));
	unsigned int cash=1000;
	float bet=0;
	char playerName[20];
	printf("Hi Player\nWhat is you name?");
	scanf("%s",playerName);
	printf("Hello %s\n",playerName);
	while(1)
	{
		printf("your amount of cash %d\n",cash);
		while(bet==0)
		{
			printf("how much willing to bet?");
            //_flushall();
			scanf("%f",&bet);
			if(bet>0 && bet<=cash)
			{
				bet=(int)bet;
				printf("your bet on %d\n",(int)bet);
			    cash-=bet;
				break;
			}
			
			if(bet<=0)
            {
                printf("not allowed negative number or zero\n");
            }
            else
            {
                printf("Wrong max amount of %d\n",cash);
            }
            bet=0;

		}
		printf("Game started\n--------------\n");
		result gameResult=game(playerName);
		printf("\n%s finel cards\n----------\n",playerName);
		show_cards(player,yes);
		printf("%s finel value %d\n",playerName,get_cards_value(player));
		printf("\ndialer finel cards\n----------\n");
		show_cards(dealer,yes);
		printf("dealer finel value %d\n",get_cards_value(dealer));
		freememmory();
		switch(gameResult)
		{
			case blackjack:
				bet=bet*2*1.5;
				cash+=bet;
				bet=0;
				printf("\nblack jack\n");
			break;
			case bust:
				bet=0;
				printf("\nyou busted\n");
			break;
			case youlose:
				bet=0;
				printf("\nyou lose\n");
			break;
			case youwin:
				cash+=bet*2;
				bet=0;
				printf("\nyou win\n");
			break;
			case dealerwins:
				bet=0;
				printf("\ndealer wins\n");
			break;
			case dealerbust:
				bet*=2;
				cash+=bet;
				bet=0;
				printf("\ndealer busted\n");
			break;
			case tie:
				cash+=bet;
				bet=0;
				printf("\ntie\n");
			break;
		}
		char answer=' ';
		while(answer!='y' && answer!='n')
        {
            printf("\ndo you want to play again? y/n\n");
            scanf("%c",&answer);
            scanf("%c",&answer);
            switch(answer)
            {
            case 'n':
                 printf("good bye\n");
                break;
                case 'y':
                 printf("welcome beck\n");
                break;
            default:
                printf("wrong please enter y/n");
                break;
            }

        }
        if(answer=='n')
        {
            break;
        }
        if(cash<10)
		{
			printf("Your out of monny\nGAME OVER\n");
			return 1;
		}

	}

}


int game(char* playerName)
{
	int chooise;
    int r;
	initcards(playerName);
	while(get_cards_value(player)<21 && chooise!=2  )
	{
		printf("\nchoose\n1.hit\n2.stand\n");
		scanf("%d",&chooise);
		switch(chooise)
		{
			case 1:
				r=rand()%CountCardInDeck();
				add_last_to_player(Remove_card_deck(r));
				printf("\n%s cards\n---------\n",playerName);
				show_cards(player,yes);
			break;
			case 2:
				chooise=2;
			break;
			default:
				printf("Wrong choise please enter 1 for hit or 2 for stand\n");
			break;
		}
	}
	if(get_cards_value(player)>21)
	{
		return bust;
	}
	if(get_cards_value(player)==21)
	{
		return blackjack;
	}
	while(get_cards_value(dealer)<17 && get_cards_value(dealer)<get_cards_value(player))
	{
		r=rand()%CountCardInDeck();
		add_last_to_dealer(Remove_card_deck(r));
	}
	if(get_cards_value(dealer)>21)
	{
		return dealerbust;
	}
	else if(get_cards_value(dealer)>get_cards_value(player))
	{
		return youlose;
	}
	else if(get_cards_value(dealer)<get_cards_value(player))
	{
		return youwin;
	}
	else if(get_cards_value(dealer)<=21 && get_cards_value(dealer)==get_cards_value(player))
	{
		return tie;
	}
	else
	{
		return dealerwins;
	}
}

void initcards(char* playerName)
{
	for(int j=0;j<13;j++)
    {
        for(int i=0;i<4;i++)
        {
             Card* newcard =malloc(sizeof(Card));
             newcard->cardType=GetCardType(i);
             newcard->CardRank=GetCardRank(j);
             newcard->CardValue=GetCardValue(j);
             add_last_to_deck(newcard);
        }
    }
    for(int i=0;i<4;i++)
    {
		int r=rand()%CountCardInDeck();
		if(i%2)
		{
			add_last_to_dealer(Remove_card_deck(r));
		}
		else
		{
			add_last_to_player(Remove_card_deck(r));
		}
	}
	printf("\n%s hand\n----------\n",playerName);
    show_cards(player,yes);
    printf("\ndealer hand\n----------\n");
    show_cards(dealer,no);
}

void freememmory()
{
	free_list(deck);
    free_list(player);
    free_list(dealer);
    //free(deck);
    deck=NULL;
    //free(player);
    player=NULL;
    //free(dealer);
    dealer=NULL;
}



char* GetCardType(int iCardType)
{
    char* Suits[4] = {
        "Hearts",
        "Clubs",
        "Diamonds",
        "Spades"
    };
    return Suits [ iCardType % 4 ];
}

char* GetCardRank(int iCardRank)
{
    char* Ranks[13] = {
        "ace",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "10",
        "prince",
        "queen",
        "king"
    };
    return Ranks [ iCardRank % 13 ];
}

int GetCardValue(int iCardValue)
{
    int Values[13] = {
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        10,
        10,
        10,
        10
    };
    return Values [ iCardValue % 13 ];
}

int get_cards_value(Card* card_list)
{
    int sum=0;
    Card* head=card_list;
    while(head!=NULL)
    {
        sum+=head->CardValue;
        head=head->next;
    }
    return sum;
}

void add_last_to_deck(Card* card)
{
    Card* head=deck;
    if(head==NULL)
    {
      deck=card;
      deck->next=NULL;
    }
    else
    {
        while(head->next!=NULL)
        {
            head=head->next;
        }
       head->next=card;
       head->next->next=NULL;


    }
}

void add_last_to_player(Card* card)
{
    Card* head=player;
    if(head==NULL)
    {
      player=card;
      player->next=NULL;
      if(player->CardValue==1)
      {
		  player->CardValue+=10;
	  }
    }
    else
    {
		while(head->next!=NULL)
        {
            head=head->next;
        }
		head->next=card;
		head->next->next=NULL;
		 if(get_cards_value(player)<21 && head->next->CardValue==1)
       {
		   head->next->CardValue+=10;
	   }
    }
}

void add_last_to_dealer(Card* card)
{
    Card* head=dealer;
    if(head==NULL)
    {
      dealer=card;
      dealer->next=NULL;
      if(dealer->CardValue==1)
      {
		  dealer->CardValue=10;
	  }
    }
    else
    {
        while(head->next!=NULL)
        {
            head=head->next;
        }
       head->next=card;
       head->next->next=NULL;
       if(get_cards_value(dealer)<21 && head->next->CardValue==1)
       {
		   head->next->CardValue+=10;
	   }
    }
}

void show_cards(Card * cards,int dontshowcard2)
{
	int counter=0;
    Card* head=cards;
    while(head!=NULL)
    {
		counter++;
		if(counter==2 && dontshowcard2==no)
		{
			printf("????????\n");
			break;
		}
        printf("%s of %s\n",head->CardRank,head->cardType);
        head=head->next;
    }
    printf("\n");
}


Card* Remove_card_deck(int position)
{
    Card* head=deck;
    Card* temp=NULL;
    if(position==0)
    {
        temp=head->next;
        head->next=NULL;
        deck=temp;
        return head;
    }
    else
    {
        temp=head->next;
        for(int i=0;i<position-1;i++)
        {
            if(temp->next==NULL)
            {
                Card* curcard=temp;
                head->next=NULL;
                return curcard;
            }
            temp=temp->next;
            head=head->next;
        }
        Card* curcard=temp;
        head->next=temp->next;
        curcard->next=NULL;
        return curcard;
    }
}

void free_list(Card* card_list)
{
	Card* head=card_list;
	while(head!=NULL)
	{
		Card* nextcard=head->next;
		head->next=NULL;
		free(head);
		head=NULL;
		//if(head!=NULL)
		//{
		   head=nextcard;
		//}
	}
}

int CountCardInDeck()
{
	int counter=0;
	Card* head=deck;
	while(head!=NULL)
	{
		counter++;
		head=head->next;
	}
	return counter;
}
