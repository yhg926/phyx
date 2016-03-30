/*
 * aatocdn.cpp
 *
 *  Created on: Jun 15, 2015
 *      Author: joe
 */

#include <iostream>
#include <string>
#include <map>

using namespace std;

#include "aatocdn.h"

map <string, string> AAtoCDN::ChangeToCodon(map <string, string>& AminoAcid,
    map<string, string>& Nucleotide) {
    
    string temp = "";
    for (iter = AminoAcid.begin(); iter != AminoAcid.end(); iter++) {
        if (Nucleotide.find(iter -> first) == Nucleotide.end()) {
            cout << "Only in the AA File: " << iter -> first << endl;
        } else {
            AminoAcidSequence = iter -> second;
            NucleotideSequence = Nucleotide[iter -> first];
            for (unsigned int i=0; i < AminoAcidSequence.size(); i++) {
                if (AminoAcidSequence[i] == '-') {
                    temp += "---";
                } else {
                    temp += NucleotideSequence[0];
                    temp += NucleotideSequence[1];
                    temp += NucleotideSequence[2];
                    NucleotideSequence.erase(0, 3);
                }
            }
            CodonSequences[iter -> first] = temp;
            temp = "";
        }
    }
    return CodonSequences;
}

AAtoCDN::AAtoCDN() {
    // TODO Auto-generated constructor stub

}

