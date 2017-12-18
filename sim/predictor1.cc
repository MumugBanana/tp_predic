#include "predictor1.h"

/////////////// STORAGE BUDGET JUSTIFICATION ////////////////

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

// Constructeur du prédicteur
PREDICTOR::PREDICTOR(char *prog, int argc, char *argv[])
{
  // La trace est tjs présente, et les arguments sont ceux que l'on désire
  if (argc != 2) {
     fprintf(stderr, "usage: %s <trace> histbits countbits \n", prog);
     exit(-1);
  }

  uint32_t pcbits    = strtoul(argv[0], NULL, 0);
  uint32_t countbits = strtoul(argv[1], NULL, 0);
  //uint32_t histbits = strtoul(argv[2], NULL, 0);

  //nentries = (1 << pcbits);        // nombre d'entrées dans la table
  //pcmask   = (nentries - 1);       // masque pour n'accéder qu'aux bits significatifs de PC
  countmax = (1 << countbits) - 1; // valeur max atteinte par le compteur à saturation
  histbitsize = (1 << pcbits);

  table    = new uint32_t[histbitsize]();
  hist     = 0;
  //histbitsize = (1 << histbits);

}

uint32_t PREDICTOR::GetHistValue()
{
  return hist;
}


void PREDICTOR::UpdateHist(bool resolveDir)
{
  hist = resolveDir ? ((hist << 1) | 1) : hist << 1;
  hist = hist & (histbitsize - 1);
  //cout <<"update " <<hist << endl;

}

uint32_t PREDICTOR::GetIndex()
{
  uint32_t retValue = GetHistValue();

  return retValue;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

bool PREDICTOR::GetPrediction(UINT64 PC)
{
  uint32_t index = GetIndex();
  uint32_t v = table[index];
  return (v > (countmax / 2)) ? TAKEN : NOT_TAKEN;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void PREDICTOR::UpdatePredictor(UINT64 PC, OpType opType, bool resolveDir, bool predDir, UINT64 branchTarget)
{
    uint32_t index = GetIndex();
    uint32_t v = table[index];
    table[index] = (resolveDir == TAKEN) ? SatIncrement(v, countmax) : SatDecrement(v);
    UpdateHist(resolveDir);
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void PREDICTOR::TrackOtherInst(UINT64 PC, OpType opType, bool branchDir, UINT64 branchTarget)
{
   // This function is called for instructions which are not
   // conditional branches, just in case someone decides to design
   // a predictor that uses information from such instructions.
   // We expect most contestants to leave this function untouched.
}



/***********************************************************/
