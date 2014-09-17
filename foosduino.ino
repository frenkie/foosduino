/**
 * Dynamo Striker Table Soccer electronics
 *
 * Control the 7-segment LEDs and the Infra Red sensors of the Dynamo Striker table.
 * For the LEDs, Dynamo uses 2 UCN5832EP 32-bit shift registers
 *
 * LED layout
 *
 *                          ds6
 *                  ds1 ds4     ds8 ds12   ds15
 *                          ds7
 *    Digit 1                                         Digit 2
 *              ds2,3,5     ds9,10,11      ds 13,24
 *
 *
 * LED segment layout:
 *
 *      a
 *   f      b
 *      g
 *   e      c
 *      d
 *
 *
 * Shift Register bit layout:
 *
 *      00000000  00000000  00000000  00000000  00000000  00000000  00000000  00000000
 *  1   1a        x         DS4 b     ds2,3,5   x         DS12 c    DS12 b    DS12 a
 *  2   1b        DS4  a    x         2e        x         DS8  c    DS12 b    x
 *  4   1f        DS1  c    x         2d        x         DS12 d    x         x
 *  8   1e        DS1  g    DS1 b     2f        x         DS12 g    DS8 f     DS8 g
 *  16  1g        DS4  f    DS1 a     2g        x         DS8  d    DS4 g     DS8 a
 *  32  1d        x         DS1 f     2a        x         x         DS8 b     DS4 d
 *  64  1c        x         DS1 e     2b        ds15      x         DS12 e    DS4 c
 *  128 x         DS4  e    x         2c        ds9,10,11 DS8  e    DS12 f    DS1 d
 *
 *
 * Connector Layout
 * The Dynamo connector consists of 2 rows of 22 connections, of which only a couple
 * are used through Arduino.
 *
 * From now on the bottom side of the connector has the noticeably thick three
 * wires, orange, red and black in the middle.
 * From orange to black is from low to high numbers.
 *
 * Bottom side
 *
 * 2 Blue Infrared
 * 7 5V out from Arduino towards Infrareds
 * 8 Ground from Arduino towards Infrareds
 * 9 Power- towards the LEDs
 * 10 Power+ towards the LEDs
 * 11 Ground for the LEDs
 * 12 Data pin
 * 13 Latch pin
 * 14 Clock pin
 * 19 Power- throughput to LEDs
 *
 * Top side
 *
 * 2 Red Infrared
 * 21 Power+ throughput to LEDs
 * 22 Ground throughput to LEDs
 *
 */


const int latchPin = 8; // yellow wire
const int clockPin = 12; // purple wire
const int dataPin = 11; // green wire

const int bestOfThreePin = 3; //analog 3; unused
const int bestOfFivePin = 5; //analog 5; unused

const int redIRPin = 2; // digital Interrupt red side infra red input, orange/purple wire
const int blueIRPin = 3; // digital Interrupt blue side infra red input, brown/purple wire

const int BYTELENGTH = 8; // number of bytes that the shift register handles

/**
 * LEDs
 * Because LED segments can span more than 1 byte, each LED should define their byte values
 * for all 8 bytes, per data value. These get added up before being output to the
 * shift register.
 *
 */

int digit1_0[8] = { 111, 0, 0, 0, 0, 0, 0, 0 };
int digit1_1[8] = { 66, 0, 0, 0, 0, 0, 0, 0 };
int digit1_2[8] = { 59, 0, 0, 0, 0, 0, 0, 0 };
int digit1_3[8] = { 115, 0, 0, 0, 0, 0, 0, 0 };
int digit1_4[8] = { 86, 0, 0, 0, 0, 0, 0, 0 };
int digit1_5[8] = { 117, 0, 0, 0, 0, 0, 0, 0 };
int digit1_6[8] = { 125, 0, 0, 0, 0, 0, 0, 0 };
int digit1_7[8] = { 67, 0, 0, 0, 0, 0, 0, 0 };
int digit1_8[8] = { 127, 0, 0, 0, 0, 0, 0, 0 };
int digit1_9[8] = { 119, 0, 0, 0, 0, 0, 0, 0 };

int* digit1[10] = { digit1_0, digit1_1, digit1_2, digit1_3, digit1_4, digit1_5, digit1_6, digit1_7, digit1_8, digit1_9 };



int digit2_0[8] = { 0, 0, 0, 238, 0, 0, 0, 0 };
int digit2_1[8] = { 0, 0, 0, 192, 0, 0, 0, 0 };
int digit2_2[8] = { 0, 0, 0, 118, 0, 0, 0, 0 };
int digit2_3[8] = { 0, 0, 0, 244, 0, 0, 0, 0 };
int digit2_4[8] = { 0, 0, 0, 216, 0, 0, 0, 0 };
int digit2_5[8] = { 0, 0, 0, 188, 0, 0, 0, 0 };
int digit2_6[8] = { 0, 0, 0, 190, 0, 0, 0, 0 };
int digit2_7[8] = { 0, 0, 0, 224, 0, 0, 0, 0 };
int digit2_8[8] = { 0, 0, 0, 254, 0, 0, 0, 0 };
int digit2_9[8] = { 0, 0, 0, 252, 0, 0, 0, 0 };

int* digit2[10] = { digit2_0, digit2_1, digit2_2, digit2_3, digit2_4, digit2_5, digit2_6, digit2_7, digit2_8, digit2_9 };


int ds1_0[8] = { 0, 4, 120, 0, 0, 0, 0, 128 };
int ds1_1[8] = { 0, 4, 8, 0, 0, 0, 0, 0 };
int ds1_2[8] = { 0, 8, 88, 0, 0, 0, 0, 128 };
int ds1_3[8] = { 0, 12, 24, 0, 0, 0, 0, 128 };
int ds1_4[8] = { 0, 12, 40, 0, 0, 0, 0, 0 };
int ds1_5[8] = { 0, 12, 48, 0, 0, 0, 0, 128 };
int ds1_6[8] = { 0, 12, 112, 0, 0, 0, 0, 128 };
int ds1_7[8] = { 0, 4, 24, 0, 0, 0, 0, 0 };
int ds1_8[8] = { 0, 12, 120, 0, 0, 0, 0, 128 };
int ds1_9[8] = { 0, 12, 56, 0, 0, 0, 0, 128 };

int* ds1[10] = { ds1_0, ds1_1, ds1_2, ds1_3, ds1_4, ds1_5, ds1_6, ds1_7, ds1_8, ds1_9 };


int ds4_0[8] = { 0, 146, 1, 0, 0, 0, 0, 96 };
int ds4_1[8] = { 0, 0, 1, 0, 0, 0, 0, 64 };
int ds4_2[8] = { 0, 130, 1, 0, 0, 0, 16, 32 };
int ds4_3[8] = { 0, 2, 1, 0, 0, 0, 16, 96 };
int ds4_4[8] = { 0, 16, 1, 0, 0, 0, 16, 64 };
int ds4_5[8] = { 0, 18, 0, 0, 0, 0, 16, 96 };
int ds4_6[8] = { 0, 146, 0, 0, 0, 0, 16, 96 };
int ds4_7[8] = { 0, 2, 1, 0, 0, 0, 0, 64 };
int ds4_8[8] = { 0, 146, 1, 0, 0, 0, 16, 96 };
int ds4_9[8] = { 0, 18, 1, 0, 0, 0, 16, 96 };

int* ds4[10] = { ds4_0, ds4_1, ds4_2, ds4_3, ds4_4, ds4_5, ds4_6, ds4_7, ds4_8, ds4_9 };

int ds8_0[8] = { 0, 0, 0, 0, 0, 146, 40, 16 };
int ds8_b[8] = { 0, 0, 0, 0, 0, 146, 8, 8 };
int* ds8[2] = {ds8_0, ds8_b}; //currently only used to display 0 or b(est of)


int ds12_0[8] = { 0, 0, 0, 0, 0, 5, 194, 1 };
int ds12_1[8] = { 0, 0, 0, 0, 0, 1, 2, 0 };
int ds12_2[8] = { 0, 0, 0, 0, 0, 12, 66, 1 };
int ds12_3[8] = { 0, 0, 0, 0, 0, 13, 2, 1 };
int ds12_4[8] = { 0, 0, 0, 0, 0, 9, 130, 0 };
int ds12_5[8] = { 0, 0, 0, 0, 0, 13, 128, 1 };
int ds12_6[8] = { 0, 0, 0, 0, 0, 13, 192, 1 };
int ds12_7[8] = { 0, 0, 0, 0, 0, 1, 2, 1 };
int ds12_8[8] = { 0, 0, 0, 0, 0, 13, 129, 1 };
int ds12_9[8] = { 0, 0, 0, 0, 0, 13, 65, 1 };

int* ds12[10] = { ds12_0, ds12_1, ds12_2, ds12_3, ds12_4, ds12_5, ds12_6, ds12_7, ds12_8, ds12_9 };


/**
 * GAME variables
 */

int resetBytes[8] = { 0, 0, 0, 0, 0, 0, 0, 0 }; // 8 clear bytes


//GAME Types
const int BESTOF3 = 3; // best of 3 DEFAULT
const int BESTOF5 = 5; // best of 5

int gameType;

int bestOfThreeValue = 0; // 
int bestOfFiveValue = 0; //

int setThreshold;
int scoreThreshold;
int maxScore;

int currentBytes[8]; // current values of the shift register's 8 bytes

volatile boolean dirty = false; // should the registers be written
boolean gameover = false; // if true, triggers the end 'ceremony' :)


volatile int redScore = 0; // digit 1
int redSets = 0; // ds1 and ds4

volatile int blueScore = 0; // digit 2
int blueSets = 0; // ds8 and ds12

volatile int debugger = 1024;

/**
 * The GAME
 */

void setup() {

    //explicitly set the digital io's on output
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);

    Serial.begin(9600); // for debugging

    setGameType( BESTOF3 );
    resetGameValues();

    delay( 1000 );

    addInterrupts(); // for the Infra red sensors
 
    debug("starting");
}

void loop() {

    if ( dirty ) {

        debug("dirty");      
      
        checkSets();

        dirty = false;

        calculateCurrentBytes();

        registerWrite( currentBytes );
    }
    
    
    if ( debugger < 1000 ) {
      Serial.println( debugger );
      debugger = 1024;
    }

    if ( gameover ) {

        debug( "we have a winner" );

        gameover = false;

        delay( 3000 );
        
        resetGameValues();

    }else{

        delay( 25 );
    }
}

void debug ( char msg[] ) {
   Serial.println( msg );
}

void addInterrupts () {

   attachInterrupt( 0, addBlueScore, FALLING );  
   attachInterrupt( 1, addRedScore, FALLING );
}

void setGameType ( int newGameType) {

    switch ( newGameType ) {

        case BESTOF5:

            gameType = BESTOF5;

            setThreshold = 2;
            scoreThreshold = 5;
            maxScore = 8;

            break;


        case BESTOF3: default:

            gameType = BESTOF3;

            setThreshold = 1;
            scoreThreshold = 5;
            maxScore = 8;            

            break;
    }
}

/**
 * This method sends bits to the shift registers:
 * Because of the shifting of the registers, the last byte should go in first, so
 * the input array is travelled reversed
 *
 * @param {int} byteArray with BYTELENGTH byte values to send to the shift registers
 */
void registerWrite ( int byteArray[] ) {

    // turn off the output so the pins don't light up
    // while you're shifting bits:
    digitalWrite(latchPin, LOW);

    for( int i = BYTELENGTH-1; i >= 0; i-- ){

        shiftOut(dataPin, clockPin, MSBFIRST, byteArray[i] );
    }

    // turn on the output so the LEDs can light up:
    digitalWrite(latchPin, HIGH);
}


void resetGameValues () {

    currentBytes[0] =  0;
    currentBytes[1] =  0;
    currentBytes[2] =  0;
    currentBytes[3] =  0;
    currentBytes[4] =  0;
    currentBytes[5] =  0;
    currentBytes[6] =  0;
    currentBytes[7] =  0;    

    dirty = true; //to start drawing the initial scores
    gameover = false;

    redScore = 0;
    redSets = 0;

    blueScore = 0;
    blueSets = 0;
}



void addBlueScore () {

    blueScore++;

    dirty = true;

    debug( "blue scored" );
}

void addRedScore () {

    redScore++;

    dirty = true;

    debug( "red scored" );
}

void checkSets () {

        // a final set should be played with 2 points difference until a maximum of points
    if ( redSets == setThreshold && blueSets == setThreshold ){

        if ( redScore == maxScore || ( redScore >= scoreThreshold && blueScore <= redScore - 2 ) ) {

            addRedSet();

        }else if ( blueScore == maxScore || ( blueScore >= scoreThreshold && redScore <= blueScore - 2 ) ) {

            addBlueSet();
        }

    }else{

        if( redScore == scoreThreshold ){

            addRedSet();

        }else if ( blueScore == scoreThreshold ){

            addBlueSet();
        }
    }

    if( redSets == setThreshold + 1 || blueSets == setThreshold + 1 ) {

        gameover = true;
    }
}

void addRedSet () {

    redSets++;
    redScore = 0;

    blueScore = 0;

    debug( "red gets a set" );
}

void addBlueSet () {

    blueSets++;
    blueScore = 0;

    redScore = 0;

    debug( "blue gets a set" );    
}


void calculateCurrentBytes( ) {

    currentBytes[0] =  0;
    currentBytes[1] =  0;
    currentBytes[2] =  0;
    currentBytes[3] =  0;
    currentBytes[4] =  0;
    currentBytes[5] =  0;
    currentBytes[6] =  0;
    currentBytes[7] =  0; 
    
    augmentBytes( currentBytes, digit1[ redScore ] );
    augmentBytes( currentBytes, digit2[ blueScore ]);
    
    augmentBytes( currentBytes, ds1[ redSets ] );
    augmentBytes( currentBytes, ds4[ blueSets ] );
    
    augmentBytes( currentBytes, ds8[ 1 ] ); // b(est of)
    augmentBytes( currentBytes, ds12[ gameType ] );    
}

void augmentBytes ( int target[], int source[] ) {

    for( int i=0; i < BYTELENGTH; i++ ){

        target[i] = target[i] + source[i];
    }    
}
