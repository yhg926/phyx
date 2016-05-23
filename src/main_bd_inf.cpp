/*
 * main_bd.cpp
 *
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <getopt.h>

using namespace std;

#include "tree_reader.h"
#include "tree.h"
#include "tree_utils.h"
#include "utils.h"
#include "bd_inf.h"

void print_help () {
    cout << "Birth-death inference" << endl;
    cout << endl;
    cout << "Usage: pxbdinf [OPTION]... " << endl;
    cout << endl;
    cout << " -t, --treef=FILE    input treefile, stdin otherwise" << endl;
    cout << " -m, --model=STRING  diversification model; either 'yule' or 'bd' (default)" << endl;
    cout << " -o, --outf=FILE     output file, stout otherwise" << endl;
    cout << " -x, --seed=INT      random number seed, clock otherwise" << endl;
    cout << "     --help          display this help and exit" << endl;
    cout << "     --version       display version and exit" << endl;
    cout << endl;
    cout << "Report bugs to: <https://github.com/FePhyFoFum/phyx/issues>" << endl;
    cout << "phyx home page: <https://github.com/FePhyFoFum/phyx>" << endl;
}

string versionline("pxbdinf 0.1\nCopyright (C) 2016 FePhyFoFum\nLicense GPLv2\nwritten by Joseph W. Brown, Stephen A. Smith (blackrim)");

static struct option const long_options[] =
{
    {"treef", required_argument, NULL, 't'},
    {"model", required_argument, NULL, 'm'},
    {"outf", required_argument, NULL, 'o'},
    {"showd", no_argument, NULL, 's'},
    {"seed", required_argument, NULL, 'x'},
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'V'},
    {NULL, 0, NULL, 0}
};

int main(int argc, char * argv[]) {
    bool outfileset = false;
    bool tfileset = false;
    
    char * treef;
    char * outf;
    
    string model = "yule";
    
    int seed = -1;
    while (1) {
        int oi = -1;
        int c = getopt_long(argc, argv, "t:m:o:x:hV", long_options, &oi);
        if (c == -1) {
            break;
        }
        switch(c) {
            case 't':
                tfileset = true;
                treef = strdup(optarg);
                check_file_exists(treef);
                break;
            case 'm':
                // need to check valid models here
                model = strdup(optarg);
                break;
            case 'o':
                outfileset = true;
                outf = strdup(optarg);
                break;
            case 'x':
                seed = atoi(strdup(optarg));
                break;
            case 'h':
                print_help();
                exit(0);
            case 'V':
                cout << versionline << endl;
                exit(0);
            default:
                print_error(argv[0], (char)c);
                exit(0);
        }
    }
    
    istream* pios;
    ostream* poos;
    ifstream* fstr;
    ofstream* ofstr;

    if (outfileset == true) {
        ofstr = new ofstream(outf);
        poos = ofstr;
    } else {
        poos = &cout;
    }
    if (tfileset == true) {
        fstr = new ifstream(treef);
        pios = fstr;
    } else {
        pios = &cin;
    }
    
    string retstring;
    int ft = test_tree_filetype_stream(*pios, retstring);
    if (ft != 0 && ft != 1) {
        cerr << "this really only works with nexus or newick" << endl;
        exit(0);
    }
    
    bool going = true;
    Tree * tree;
    tree = read_next_tree_from_stream_newick (*pios, retstring, &going);
    
    double treelength = get_tree_length(tree);
    double numnodes = tree->getInternalNodeCount();
    double numspeciations = numnodes - 1.0;
    
    // ML estimate of lambda
    double lambda = numspeciations / treelength;
    
    double PI = 3.1415926535897932384626433832795;
    double x = numnodes + 1;
    double lfact_n = (x - 0.5)*log(x) - x + 0.5*log(2*PI) + 1.0/(12.0*x);
    
    double lik = numspeciations * log(lambda) - lambda * treelength + lfact_n;
    
    cout << endl << "num tips: " << tree->getExternalNodeCount() << endl;
    cout << "tree length: " << treelength << endl;
    cout << "lambda: " << lambda << endl;
    cout << "likelihood: " << lik << endl;
    
    delete tree;
    
    /*
    BirthDeathSimulator bd(ext, time, birth, death, seed);
    for (int i = 0; i < nreps; i++) {
        Tree * bdtr = bd.make_tree(showd);
        
        if (ext != 0) {
            int countlimit = 100;
            int count = 1;
            // the following doesn't get called. the bd sim handles failures itself.
            while (bdtr->getExtantNodeCount() != ext) {
                delete (bdtr);
                bdtr = bd.make_tree(showd);
                if (count >= countlimit) {
                    cout << "can't seem to get the tips right after " << countlimit << " trials" << endl;
                    break;
                }
                count ++;
            }
        }
        
        (*poos) << bdtr->getRoot()->getNewick(true) << ";" << endl;
    }
    */
    if (outfileset) {
        ofstr->close();
        delete poos;
    }
    return EXIT_SUCCESS;
}
