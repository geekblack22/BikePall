#include "Voice.h"

int Voice::voiceInput(){
    return analogRead(mic);
}

