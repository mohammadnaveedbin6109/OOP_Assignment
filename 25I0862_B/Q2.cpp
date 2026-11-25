#include"pch.h"
#include <iostream>
#include <cstring>
#include <cstdint>
#include <cstdio>
using namespace std;

class Cell {
private:
    static constexpr int MAX_SPECIES_LENGTH = 20;
    static constexpr int MAX_CHROMOSOMES = 16;
    static constexpr int MAX_GENES = 12;
    static constexpr int MAX_TELOMERE = 50;
    static constexpr int DEFAULT_TELOMERE = 12;

public:
    class Chromosome {          // nested class
    public:
        struct Gene {           // nested struct, public data
            char allele;
            int locus;
            Gene() {            // allele '?', locus -1
                this->allele = '?';
                this->locus = -1;
            }
            Gene(char allele, int locus) {
                this->allele = allele;
                this->locus = locus;
            }
            bool isDominant() const {       // upper-case letter
                return allele >= 'A' && allele <= 'Z';
            }
            char trait() const {            // the upper-case form of the letter
                if (allele >= 'a' && allele <= 'z') return (char)(allele - 'a' + 'A');
                return allele;
            }
            Gene& flipCase() {              // 'a' <-> 'A', returns *this
                if (allele >= 'a' && allele <= 'z') allele = (char)(allele - 'a' + 'A');
                else if (allele >= 'A' && allele <= 'Z') allele = (char)(allele - 'A' + 'a');
                return *this;
            }
        };

        Chromosome() : id(0), geneCount(0), telomere(0), genes(nullptr) {   // empty chromosome
            ++liveCount;
        }

        // genes start as Gene(); telomere clamped to 0..50
        Chromosome(int id, int geneCount, int telomere) : genes(nullptr) {
            this->id = id;
            this->geneCount = geneCount > 0 ? geneCount : 0;
            this->telomere = clampTelomere(telomere);
            if (this->geneCount > 0) genes = new Gene[this->geneCount];
            ++liveCount;
        }

        Chromosome(const Chromosome& other)
            : id(other.id), geneCount(other.geneCount), telomere(other.telomere), genes(nullptr) {
            if (geneCount > 0) {
                genes = new Gene[geneCount];
                for (int i = 0; i < geneCount; ++i) genes[i] = other.genes[i];
            }
            ++liveCount;
        }

        ~Chromosome() {
            delete[] genes;
            --liveCount;
        }

        int getId() const { return id; }
        int getGeneCount() const { return geneCount; }
        int getTelomere() const { return telomere; }
        void setTelomere(int t) { telomere = clampTelomere(t); }   // clamps to 0..50
        bool senescent() const { return telomere <= 0; }

        // telomere -= amount, result clamped to 0..50, returns *this
        Chromosome& shorten(int amount) {
            telomere = clampTelomere(telomere - amount);
            return *this;
        }

        // bad locus: undefined (callers must not do it)
        Gene& gene(int locus) { return genes[locus]; }
        const Gene& gene(int locus) const { return genes[locus]; }

        static int live() { return liveCount; }   // chromosomes currently alive

    private:
        static int clampTelomere(int t) {
            if (t < 0) return 0;
            if (t > MAX_TELOMERE) return MAX_TELOMERE;
            return t;
        }

        int id;
        int geneCount;
        int telomere;
        Gene* genes;
        static int liveCount;
        Chromosome& operator=(const Chromosome&);   // declared, never defined
    };

    // invalid cell, species "UNKNOWN"
    Cell();
    Cell(const char* species, int ploidy, int chromosomes, int genesPerChromosome,
        int telomere, const char* genome);
    // telomere defaults to 12
    Cell(const char* species, int ploidy, int chromosomes, int genesPerChromosome,
        const char* genome);
    Cell(const Cell& other);
    ~Cell();

    bool isValid() const;
    const char* getSpecies() const;
    int getPloidy() const;
    int getChromosomeCount() const;
    int getGenesPerChromosome() const;

    // -1 for a bad index or invalid cell
    int getTelomere(int chromosome) const;
    // clamps to 0..50, false on bad index
    bool setTelomere(int chromosome, int value);

    // valid index on a valid cell only
    Chromosome& getChromosome(int index);
    const Chromosome& getChromosome(int index) const;

    // new[]'d, caller delete[]s
    char* genomeString() const;
    char phenotype(int pair, int locus) const;
    int totalLoci() const;
    int dominantCount() const;
    double dominantRatio() const;
    int countAllele(char allele) const;                  // whole cell
    int countAllele(char allele, int chromosome) const;  // one chromosome
    bool sameGenome(const Cell& other) const;

    // uses the shared seed
    Cell& irradiate(int bp);
    Cell& irradiate(int bp, uint64_t seed);              // uses a private seed

    // shared seed, shared error rate
    Cell* mitosis();
    // private seed, given rate
    Cell* mitosis(uint64_t seed, int bp);

    Cell** meiosis(int& produced) const;                 // shared seed
    Cell** meiosis(int& produced, uint64_t seed) const;  // private seed

    static Cell* fertilize(const Cell& father, const Cell& mother);

    void print() const;

    static void setSeed(uint64_t s);
    static uint64_t getSeed();
    // clamp 0..10000, default 100
    static void setCopyErrorRate(int bp);
    static int getCopyErrorRate();
    // clamp 0..100, default 50
    static void setCrossoverPercent(int p);
    static int getCrossoverPercent();

    static int getLiveCells();
    static int getTotalCreated();
    static int getTotalMutations();
    static int getTotalCrossovers();
    static int getRefusedDivisions();
    static void resetStatistics();

private:
    // ---- helpers ----
    static char* dupString(const char* text);
    static bool inputsValid(const char* sp, int pl, int ch, int g, int tel, const char* gen);
    static int clampBasisPoints(int value);
    static uint64_t nextDraw(uint64_t& state);
    static bool isLetter(char c);
    static char lowerOf(char c);
    void copyErrorPass(uint64_t& state, int bp);
    Cell* divide(uint64_t& state, int bp);
    Cell** makeGametes(int& produced, uint64_t& state) const;

    // ---- data ----
    char* const species;
    int ploidy;
    int chromosomes;
    int genesPerChromosome;
    Chromosome** chromosomeList;
    bool valid;

    static uint64_t sharedSeed;
    static int copyErrorRate;
    static int crossoverPercent;
    static int liveCells;
    static int totalCreated;
    static int totalMutations;
    static int totalCrossovers;
    static int refusedDivisions;

    // declared, never defined
    Cell& operator=(const Cell&);
};

// ----------------------------------------------------------------------
// static data members
// ----------------------------------------------------------------------
int Cell::Chromosome::liveCount = 0;
uint64_t Cell::sharedSeed = 0;
int Cell::copyErrorRate = 100;
int Cell::crossoverPercent = 50;
int Cell::liveCells = 0;
int Cell::totalCreated = 0;
int Cell::totalMutations = 0;
int Cell::totalCrossovers = 0;
int Cell::refusedDivisions = 0;

// ----------------------------------------------------------------------
// private helpers
// ----------------------------------------------------------------------
char* Cell::dupString(const char* text) {
    const char* src = (text == nullptr) ? "UNKNOWN" : text;
    char* copy = new char[std::strlen(src) + 1];
    std::strcpy(copy, src);
    return copy;
}

bool Cell::isLetter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

char Cell::lowerOf(char c) {
    if (c >= 'A' && c <= 'Z') return (char)(c - 'A' + 'a');
    return c;
}

int Cell::clampBasisPoints(int value) {
    if (value < 0) return 0;
    if (value > 10000) return 10000;
    return value;
}

uint64_t Cell::nextDraw(uint64_t& state) {
    state = state * 6364136223846793005ULL + 1442695040888963407ULL;
    return state;
}

bool Cell::inputsValid(const char* sp, int pl, int ch, int g, int tel, const char* gen) {
    if (sp == nullptr) return false;
    int len = (int)std::strlen(sp);
    if (len < 1 || len > MAX_SPECIES_LENGTH) return false;
    for (int i = 0; i < len; ++i) {
        char c = sp[i];
        bool ok = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') || c == '_';
        if (!ok) return false;
    }
    if (pl != 1 && pl != 2) return false;
    if (ch < 1 || ch > MAX_CHROMOSOMES) return false;
    if (pl == 2 && ch % 2 != 0) return false;
    if (g < 1 || g > MAX_GENES) return false;
    if (tel < 0 || tel > MAX_TELOMERE) return false;
    if (gen == nullptr) return false;
    if ((int)std::strlen(gen) != ch * g) return false;
    for (int i = 0; i < ch * g; ++i)
        if (!isLetter(gen[i])) return false;
    if (pl == 2) {
        for (int p = 0; p < ch / 2; ++p)
            for (int l = 0; l < g; ++l)
                if (lowerOf(gen[(2 * p) * g + l]) != lowerOf(gen[(2 * p + 1) * g + l]))
                    return false;
    }
    return true;
}

// one draw per gene, in chromosome/gene order; flips when the draw says so
void Cell::copyErrorPass(uint64_t& state, int bp) {
    if (!valid) return;
    const uint64_t limit = (uint64_t)bp;
    for (int c = 0; c < chromosomes; ++c) {
        for (int l = 0; l < genesPerChromosome; ++l) {
            uint64_t d = nextDraw(state);
            if (((d >> 33) % 10000) < limit) {
                chromosomeList[c]->gene(l).flipCase();
                ++totalMutations;
            }
        }
    }
}

Cell* Cell::divide(uint64_t& state, int bp) {
    if (!valid) return new Cell();
    for (int i = 0; i < chromosomes; ++i) {
        if (chromosomeList[i]->senescent()) {
            ++refusedDivisions;
            return new Cell();
        }
    }
    for (int i = 0; i < chromosomes; ++i) chromosomeList[i]->shorten(2);
    Cell* daughter = new Cell(*this);
    daughter->copyErrorPass(state, clampBasisPoints(bp));
    return daughter;
}

Cell** Cell::makeGametes(int& produced, uint64_t& state) const {
    if (!valid || ploidy != 2) {
        produced = 0;
        return nullptr;
    }
    for (int i = 0; i < chromosomes; ++i) {
        if (chromosomeList[i]->senescent()) {
            ++refusedDivisions;
            produced = 0;
            return nullptr;
        }
    }

    const int pairs = chromosomes / 2;
    const int g = genesPerChromosome;

    // 1. work space: plain char array, chromosome after chromosome
    char* work = new char[chromosomes * g];
    for (int i = 0; i < chromosomes; ++i)
        for (int l = 0; l < g; ++l)
            work[i * g + l] = chromosomeList[i]->gene(l).allele;

    // 2. crossover inside each homologous pair
    for (int p = 0; p < pairs; ++p) {
        uint64_t d = nextDraw(state);
        if (g >= 2 && ((d >> 33) % 100) < (uint64_t)crossoverPercent) {
            int cp = 1 + (int)((d >> 20) % (uint64_t)(g - 1));
            for (int l = cp; l < g; ++l) {
                char tmp = work[(2 * p) * g + l];
                work[(2 * p) * g + l] = work[(2 * p + 1) * g + l];
                work[(2 * p + 1) * g + l] = tmp;
            }
            ++totalCrossovers;
        }
    }

    // 3. independent assortment into two poles
    int* pole1 = new int[pairs];
    int* pole2 = new int[pairs];
    for (int p = 0; p < pairs; ++p) {
        uint64_t d = nextDraw(state);
        if (((d >> 40) & 1) == 0) {
            pole1[p] = 2 * p;
            pole2[p] = 2 * p + 1;
        }
        else {
            pole1[p] = 2 * p + 1;
            pole2[p] = 2 * p;
        }
    }

    // 4. four haploid gametes: 0,1 from pole 1; 2,3 from pole 2
    char* buffer = new char[pairs * g + 1];
    Cell** gametes = new Cell * [4];
    for (int k = 0; k < 4; ++k) {
        const int* pole = (k < 2) ? pole1 : pole2;
        for (int i = 0; i < pairs; ++i)
            for (int l = 0; l < g; ++l)
                buffer[i * g + l] = work[pole[i] * g + l];
        buffer[pairs * g] = '\0';
        gametes[k] = new Cell(species, 1, pairs, g, DEFAULT_TELOMERE, buffer);
        for (int i = 0; i < pairs; ++i) {
            int t = chromosomeList[pole[i]]->getTelomere() - 1;
            if (t < 0) t = 0;
            gametes[k]->chromosomeList[i]->setTelomere(t);
        }
    }

    // 5. copy errors on gamete 0, 1, 2, 3 in order
    for (int k = 0; k < 4; ++k) gametes[k]->copyErrorPass(state, copyErrorRate);

    delete[] buffer;
    delete[] pole2;
    delete[] pole1;
    delete[] work;
    produced = 4;
    return gametes;
}


// constructors / destructor

Cell::Cell()
    : species(dupString(nullptr)), ploidy(0), chromosomes(0), genesPerChromosome(0),
    chromosomeList(nullptr), valid(false) {
    ++liveCells;                 // the default cell is alive but not "created"
}

Cell::Cell(const char* species, int ploidy, int chromosomes, int genesPerChromosome,
    int telomere, const char* genome)
    : species(dupString(species)), ploidy(0), chromosomes(0), genesPerChromosome(0),
    chromosomeList(nullptr), valid(false) {
    ++liveCells;
    ++totalCreated;
    if (!inputsValid(species, ploidy, chromosomes, genesPerChromosome, telomere, genome))
        return;                  // stays an invalid cell
    this->ploidy = ploidy;
    this->chromosomes = chromosomes;
    this->genesPerChromosome = genesPerChromosome;
    chromosomeList = new Chromosome * [chromosomes];
    for (int i = 0; i < chromosomes; ++i) {
        chromosomeList[i] = new Chromosome(i, genesPerChromosome, telomere);
        for (int l = 0; l < genesPerChromosome; ++l)
            chromosomeList[i]->gene(l) = Chromosome::Gene(genome[i * genesPerChromosome + l], l);
    }
    valid = true;
}

Cell::Cell(const char* species, int ploidy, int chromosomes, int genesPerChromosome,
    const char* genome)
    : Cell(species, ploidy, chromosomes, genesPerChromosome, DEFAULT_TELOMERE, genome) {
}

Cell::Cell(const Cell& other)
    : species(dupString(other.species)), ploidy(other.ploidy), chromosomes(other.chromosomes),
    genesPerChromosome(other.genesPerChromosome), chromosomeList(nullptr), valid(other.valid) {
    ++liveCells;
    ++totalCreated;
    if (other.chromosomeList != nullptr) {
        chromosomeList = new Chromosome * [chromosomes];
        for (int i = 0; i < chromosomes; ++i)
            chromosomeList[i] = new Chromosome(*other.chromosomeList[i]);
    }
}

Cell::~Cell() {
    for (int i = 0; i < chromosomes; ++i) delete chromosomeList[i];
    delete[] chromosomeList;
    delete[] species;
    --liveCells;
}

// ----------------------------------------------------------------------
// simple getters
// ----------------------------------------------------------------------
bool Cell::isValid() const { return valid; }
const char* Cell::getSpecies() const { return species; }
int Cell::getPloidy() const { return ploidy; }
int Cell::getChromosomeCount() const { return chromosomes; }
int Cell::getGenesPerChromosome() const { return genesPerChromosome; }

int Cell::getTelomere(int chromosome) const {
    if (!valid || chromosome < 0 || chromosome >= chromosomes) return -1;
    return chromosomeList[chromosome]->getTelomere();
}

bool Cell::setTelomere(int chromosome, int value) {
    if (!valid || chromosome < 0 || chromosome >= chromosomes) return false;
    chromosomeList[chromosome]->setTelomere(value);
    return true;
}

Cell::Chromosome& Cell::getChromosome(int index) {
    return *chromosomeList[index];
}

const Cell::Chromosome& Cell::getChromosome(int index) const {
    return *chromosomeList[index];
}

// queries

char* Cell::genomeString() const {
    if (!valid) {
        char* na = new char[4];
        std::strcpy(na, "N/A");
        return na;
    }
    int len = chromosomes * genesPerChromosome + (chromosomes - 1);
    char* text = new char[len + 1];
    int pos = 0;
    for (int i = 0; i < chromosomes; ++i) {
        if (i > 0) text[pos++] = '-';
        for (int l = 0; l < genesPerChromosome; ++l)
            text[pos++] = chromosomeList[i]->gene(l).allele;
    }
    text[pos] = '\0';
    return text;
}

char Cell::phenotype(int pair, int locus) const {
    if (!valid || locus < 0 || locus >= genesPerChromosome || pair < 0) return '?';
    if (ploidy == 2) {
        if (pair >= chromosomes / 2) return '?';
        const Chromosome::Gene& first = chromosomeList[2 * pair]->gene(locus);
        const Chromosome::Gene& second = chromosomeList[2 * pair + 1]->gene(locus);
        if (first.isDominant() || second.isDominant()) return first.trait();
        return first.allele;
    }
    if (pair >= chromosomes) return '?';
    return chromosomeList[pair]->gene(locus).allele;
}

int Cell::totalLoci() const {
    if (!valid) return 0;
    if (ploidy == 2) return (chromosomes / 2) * genesPerChromosome;
    return chromosomes * genesPerChromosome;
}

int Cell::dominantCount() const {
    if (!valid) return 0;
    int positions = (ploidy == 2) ? chromosomes / 2 : chromosomes;
    int count = 0;
    for (int p = 0; p < positions; ++p) {
        for (int l = 0; l < genesPerChromosome; ++l) {
            char shown = phenotype(p, l);
            if (shown >= 'A' && shown <= 'Z') ++count;
        }
    }
    return count;
}

double Cell::dominantRatio() const {
    int total = totalLoci();
    if (total == 0) return 0.0;
    return (double)dominantCount() / (double)total;
}

int Cell::countAllele(char allele) const {
    if (!valid) return 0;
    int count = 0;
    for (int i = 0; i < chromosomes; ++i)
        for (int l = 0; l < genesPerChromosome; ++l)
            if (chromosomeList[i]->gene(l).allele == allele) ++count;
    return count;
}

int Cell::countAllele(char allele, int chromosome) const {
    if (!valid || chromosome < 0 || chromosome >= chromosomes) return 0;
    int count = 0;
    for (int l = 0; l < genesPerChromosome; ++l)
        if (chromosomeList[chromosome]->gene(l).allele == allele) ++count;
    return count;
}

bool Cell::sameGenome(const Cell& other) const {
    if (!valid || !other.valid) return false;
    if (std::strcmp(species, other.species) != 0) return false;
    if (ploidy != other.ploidy || chromosomes != other.chromosomes ||
        genesPerChromosome != other.genesPerChromosome)
        return false;
    for (int i = 0; i < chromosomes; ++i)
        for (int l = 0; l < genesPerChromosome; ++l)
            if (chromosomeList[i]->gene(l).allele != other.chromosomeList[i]->gene(l).allele)
                return false;
    return true;
}


// mutation and division

Cell& Cell::irradiate(int bp) {
    copyErrorPass(sharedSeed, clampBasisPoints(bp));
    return *this;
}

Cell& Cell::irradiate(int bp, uint64_t seed) {
    uint64_t state = seed;       // private generator
    copyErrorPass(state, clampBasisPoints(bp));
    return *this;
}

Cell* Cell::mitosis() {
    return divide(sharedSeed, copyErrorRate);
}

Cell* Cell::mitosis(uint64_t seed, int bp) {
    uint64_t state = seed;       // private generator
    return divide(state, bp);
}

Cell** Cell::meiosis(int& produced) const {
    return makeGametes(produced, sharedSeed);
}

Cell** Cell::meiosis(int& produced, uint64_t seed) const {
    uint64_t state = seed;       // private generator
    return makeGametes(produced, state);
}

Cell* Cell::fertilize(const Cell& father, const Cell& mother) {
    if (!father.valid || !mother.valid) return new Cell();
    if (father.ploidy != 1 || mother.ploidy != 1) return new Cell();
    if (std::strcmp(father.species, mother.species) != 0) return new Cell();
    if (father.chromosomes != mother.chromosomes ||
        father.genesPerChromosome != mother.genesPerChromosome)
        return new Cell();

    const int n = father.chromosomes;
    const int g = father.genesPerChromosome;
    char* genome = new char[2 * n * g + 1];
    for (int p = 0; p < n; ++p) {
        for (int l = 0; l < g; ++l) {
            genome[(2 * p) * g + l] = father.chromosomeList[p]->gene(l).allele;
            genome[(2 * p + 1) * g + l] = mother.chromosomeList[p]->gene(l).allele;
        }
    }
    genome[2 * n * g] = '\0';

    // the constructor decides validity (too many chromosomes, letters that disagree)
    Cell* zygote = new Cell(father.species, 2, 2 * n, g, DEFAULT_TELOMERE, genome);
    delete[] genome;
    if (zygote->valid) {
        for (int p = 0; p < n; ++p) {
            zygote->chromosomeList[2 * p]->setTelomere(father.chromosomeList[p]->getTelomere());
            zygote->chromosomeList[2 * p + 1]->setTelomere(mother.chromosomeList[p]->getTelomere());
        }
    }
    return zygote;
}


// output

void Cell::print() const {
    cout << species << " | Ploidy: " << ploidy << " | Chromosomes: " << chromosomes
        << " | Genes/Chr: " << genesPerChromosome << " | Telomeres: ";
    if (!valid) {
        cout << "-";
    }
    else {
        for (int i = 0; i < chromosomes; ++i) {
            if (i > 0) cout << ",";
            cout << chromosomeList[i]->getTelomere();
        }
    }
    char ratio[32];
    std::snprintf(ratio, sizeof(ratio), "%.2f", dominantRatio());
    char* genome = genomeString();
    cout << " | Dominant: " << dominantCount() << "/" << totalLoci() << " (" << ratio
        << ") | Valid: " << (valid ? "YES" : "NO") << " | Genome: " << genome << "\n";
    delete[] genome;
}


// statics

void Cell::setSeed(uint64_t s) { sharedSeed = s; }
uint64_t Cell::getSeed() { return sharedSeed; }

void Cell::setCopyErrorRate(int bp) { copyErrorRate = clampBasisPoints(bp); }
int Cell::getCopyErrorRate() { return copyErrorRate; }

void Cell::setCrossoverPercent(int p) {
    if (p < 0) p = 0;
    if (p > 100) p = 100;
    crossoverPercent = p;
}
int Cell::getCrossoverPercent() { return crossoverPercent; }

int Cell::getLiveCells() { return liveCells; }
int Cell::getTotalCreated() { return totalCreated; }
int Cell::getTotalMutations() { return totalMutations; }
int Cell::getTotalCrossovers() { return totalCrossovers; }
int Cell::getRefusedDivisions() { return refusedDivisions; }

void Cell::resetStatistics() {
    totalCreated = 0;
    totalMutations = 0;
    totalCrossovers = 0;
    refusedDivisions = 0;
    sharedSeed = 0;
    copyErrorRate = 100;
    crossoverPercent = 50;
}