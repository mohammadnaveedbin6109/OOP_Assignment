#include"pch.h"
#include <iostream>
#include <cstring>
#include <cstdint>

typedef uint32_t u32;
typedef uint64_t u64;


class Operation;


// Gadget

class Gadget {
public:
    Gadget(const char* name, char type, int bonus, int uses);
    Gadget(const Gadget& other);
    ~Gadget();
    const char* getName() const;
    char getType() const;
    int  getBonus() const;
    int  getUses() const;
    bool helps(char lockType) const;
    int  effectiveBonus(char lockType) const;
    void consume();                          // one use less, if any remain
    static int live();
private:
    Gadget& operator=(const Gadget&);

    char* name;
    char  type;
    int   bonus;
    int   uses;
    static int liveCount;
};

int Gadget::liveCount = 0;

Gadget::Gadget(const char* n, char t, int b, int u)
    : name(0), type(t), bonus(b), uses(u) {
    name = new char[std::strlen(n) + 1];
    std::strcpy(name, n);
    ++liveCount;
}

Gadget::Gadget(const Gadget& other)
    : name(0), type(other.type), bonus(other.bonus), uses(other.uses) {
    name = new char[std::strlen(other.name) + 1];
    std::strcpy(name, other.name);
    ++liveCount;
}

Gadget::~Gadget() {
    delete[] name;
    --liveCount;
}

const char* Gadget::getName() const { return name; }
char Gadget::getType() const { return type; }
int  Gadget::getBonus() const { return bonus; }
int  Gadget::getUses() const { return uses; }

bool Gadget::helps(char lockType) const {
    return uses > 0 && (type == lockType || type == '*');
}

int Gadget::effectiveBonus(char lockType) const {
    if (type == '*' && lockType == 'X')
        return bonus / 2;
    return bonus;
}

void Gadget::consume() {
    if (uses > 0) --uses;
}

int Gadget::live() { return liveCount; }


// Lock

class Lock {
public:
    // "lock", type 'G', p1 = 0, p2 = 0, baseTime = 1
    Lock();
    Lock(const char* name, char type, u32 p1, u32 p2, int baseTime);
    ~Lock();
    const char* getName() const;
    char getType() const;
    int getBaseTime() const;

    // stays true after a lock with code 0 has been cracked
    bool isCracked() const;

    // the code if cracked, otherwise 0
    u32 getCode() const;

    // true if candidate is the code, computed forward from p1 and p2. Does
    // not use a stored code and does not crack the lock.
    bool verify(u32 candidate) const;
    u32 crack();
    int getRefCount() const;                 // starts at 0
    void addRef();

    // count - 1; true if the count is now 0 or below
    bool release();
    static int live();
private:
    Lock(const Lock&);                       // never defined: locks are shared
    Lock& operator=(const Lock&);

    static u32 scrambleRound(u32 x);
    static u32 unscrambleRound(u32 x);

    char* name;
    char  type;
    u32   p1;
    u32   p2;
    int   baseTime;
    u32   code;
    bool  cracked;
    int   refCount;
    static int liveCount;
};

int Lock::liveCount = 0;

u32 Lock::scrambleRound(u32 x) {
    x = x ^ (x << 13);
    x = x ^ (x >> 17);
    x = x ^ (x << 5);
    return x;
}

// Exact inverse of scrambleRound: undo the three steps in reverse order.
u32 Lock::unscrambleRound(u32 x) {
    u32 y = x;
    for (int i = 0; i < 32; i += 5)  x = y ^ (x << 5);
    y = x;
    for (int i = 0; i < 32; i += 17) x = y ^ (x >> 17);
    y = x;
    for (int i = 0; i < 32; i += 13) x = y ^ (x << 13);
    return x;
}

Lock::Lock()
    : name(0), type('G'), p1(0), p2(0), baseTime(1),
    code(0), cracked(false), refCount(0) {
    name = new char[std::strlen("lock") + 1];
    std::strcpy(name, "lock");
    ++liveCount;
}

Lock::Lock(const char* n, char t, u32 a, u32 b, int bt)
    : name(0), type(t), p1(a), p2(b), baseTime(bt),
    code(0), cracked(false), refCount(0) {
    name = new char[std::strlen(n) + 1];
    std::strcpy(name, n);
    ++liveCount;
}

Lock::~Lock() {
    delete[] name;
    --liveCount;
}

const char* Lock::getName() const { return name; }
char Lock::getType() const { return type; }
int  Lock::getBaseTime() const { return baseTime; }
bool Lock::isCracked() const { return cracked; }
u32  Lock::getCode() const { return cracked ? code : 0; }
int  Lock::getRefCount() const { return refCount; }
void Lock::addRef() { ++refCount; }

bool Lock::release() {
    --refCount;
    return refCount <= 0;
}

int Lock::live() { return liveCount; }

bool Lock::verify(u32 candidate) const {
    if (type == 'M') {
        u64 m = p2;
        u64 c = candidate;
        if (c == 0 || c >= m) return false;
        return ((u64)p1 * c) % m == 1;
    }
    if (type == 'G') {
        return (candidate ^ (candidate >> 1)) == p1;
    }
    // type 'X': run p2 rounds forward from the candidate, compare with p1
    u32 x = candidate;
    for (u32 r = 0; r < p2; ++r) x = scrambleRound(x);
    return x == p1;
}

// crack() is defined after class Operation (it calls Operation::addHeat).


// Member

class Member {
public:
    Member(const char* name, int skill);
    Member(const Member& other);
    ~Member();
    const char* getName() const;
    int getSkill() const;                    // original skill
    int getJobs() const;
    int gadgetCount() const;
    const Gadget* getGadget(int index) const;
    int addGadget(const Gadget& gadget);     // stores its own copy
    int addGadget(const char* name, char type, int bonus, int uses);
    int effectiveSkill() const;
    int bonusFor(char lockType) const;
    int timeFor(const Lock& lock) const;     // changes nothing
    int work(const Lock& lock);              // returns the time it took
    static int live();
private:
    Member& operator=(const Member&);
    void grow();

    char* name;
    int      skill;
    int      jobs;
    Gadget** gadgets;
    int      count;
    int      capacity;
    static int liveCount;
};

int Member::liveCount = 0;

Member::Member(const char* n, int s)
    : name(0), skill(s), jobs(0), gadgets(0), count(0), capacity(2) {
    name = new char[std::strlen(n) + 1];
    std::strcpy(name, n);
    gadgets = new Gadget * [capacity];
    ++liveCount;
}

Member::Member(const Member& other)
    : name(0), skill(other.skill), jobs(other.jobs), gadgets(0),
    count(other.count), capacity(other.capacity) {
    name = new char[std::strlen(other.name) + 1];
    std::strcpy(name, other.name);
    gadgets = new Gadget * [capacity];
    for (int i = 0; i < count; ++i)
        gadgets[i] = new Gadget(*other.gadgets[i]);
    ++liveCount;
}

Member::~Member() {
    for (int i = 0; i < count; ++i) delete gadgets[i];
    delete[] gadgets;
    delete[] name;
    --liveCount;
}

void Member::grow() {
    int newCap = capacity * 2;
    Gadget** bigger = new Gadget * [newCap];
    for (int i = 0; i < count; ++i) bigger[i] = gadgets[i];
    delete[] gadgets;
    gadgets = bigger;
    capacity = newCap;
}

const char* Member::getName() const { return name; }
int Member::getSkill() const { return skill; }
int Member::getJobs() const { return jobs; }
int Member::gadgetCount() const { return count; }
int Member::live() { return liveCount; }

const Gadget* Member::getGadget(int index) const {
    if (index < 0 || index >= count) return nullptr;
    return gadgets[index];
}

int Member::addGadget(const Gadget& gadget) {
    if (count >= 5) return -1;
    if (count == capacity) grow();
    gadgets[count] = new Gadget(gadget);
    return count++;
}

int Member::addGadget(const char* n, char type, int bonus, int uses) {
    if (count >= 5) return -1;
    if (count == capacity) grow();
    gadgets[count] = new Gadget(n, type, bonus, uses);
    return count++;
}

int Member::effectiveSkill() const {
    int s = skill - jobs / 3;
    return s > 1 ? s : 1;
}

int Member::bonusFor(char lockType) const {
    int sum = 0;
    for (int i = 0; i < count; ++i)
        if (gadgets[i]->helps(lockType))
            sum += gadgets[i]->effectiveBonus(lockType);
    return sum > 100 ? 100 : sum;
}

int Member::timeFor(const Lock& lock) const {
    if (lock.isCracked()) return 0;
    long long num = (long long)lock.getBaseTime() * 100;
    long long den = 100 + 10LL * effectiveSkill() + bonusFor(lock.getType());
    return (int)((num + den - 1) / den);
}

int Member::work(const Lock& lock) {
    int t = timeFor(lock);
    char lt = lock.getType();
    for (int i = 0; i < count; ++i)
        if (gadgets[i]->helps(lt)) gadgets[i]->consume();
    ++jobs;
    return t;
}


// Vault

class Vault {
public:
    Vault(const char* name, u32 base, u64 exp, u32 m,
        int windowStart, int windowEnd);
    Vault(const Vault& other);               // shares the same Lock objects
    ~Vault();
    const char* getName() const;
    int windowStart() const;
    int windowEnd() const;


    int addLock(Lock* lock);

    int addLock(const char* name, char type, u32 p1, u32 p2, int baseTime);

    // Later locks move one place up. false for a bad index.
    bool removeLock(int index);
    int findLock(const char* name) const;
    int lockCount() const;
    int capacity() const;                    // never shrinks
    int crackedCount() const;
    const Lock* getLock(int index) const;
    Lock* getLock(int index);
    bool isOpen() const;
    u32 key() const;
    int noise() const;
    u32 loot() const;
    static int live();
private:
    Vault& operator=(const Vault&);
    void grow();

    char* name;
    u32    baseVal;
    u64    expVal;
    u32    modVal;
    int    winStart;
    int    winEnd;
    Lock** locks;
    int    count;
    int    cap;
    static int liveCount;
};

int Vault::liveCount = 0;

Vault::Vault(const char* n, u32 b, u64 e, u32 m, int ws, int we)
    : name(0), baseVal(b), expVal(e), modVal(m), winStart(ws), winEnd(we),
    locks(0), count(0), cap(2) {
    name = new char[std::strlen(n) + 1];
    std::strcpy(name, n);
    locks = new Lock * [cap];
    ++liveCount;
}

Vault::Vault(const Vault& other)
    : name(0), baseVal(other.baseVal), expVal(other.expVal),
    modVal(other.modVal), winStart(other.winStart), winEnd(other.winEnd),
    locks(0), count(other.count), cap(other.cap) {
    name = new char[std::strlen(other.name) + 1];
    std::strcpy(name, other.name);
    locks = new Lock * [cap];
    for (int i = 0; i < count; ++i) {
        locks[i] = other.locks[i];
        locks[i]->addRef();
    }
    ++liveCount;
}

Vault::~Vault() {
    for (int i = 0; i < count; ++i)
        if (locks[i]->release()) delete locks[i];
    delete[] locks;
    delete[] name;
    --liveCount;
}

void Vault::grow() {
    int newCap = cap * 2;
    Lock** bigger = new Lock * [newCap];
    for (int i = 0; i < count; ++i) bigger[i] = locks[i];
    delete[] locks;
    locks = bigger;
    cap = newCap;
}

const char* Vault::getName() const { return name; }
int Vault::windowStart() const { return winStart; }
int Vault::windowEnd() const { return winEnd; }
int Vault::lockCount() const { return count; }
int Vault::capacity() const { return cap; }
int Vault::live() { return liveCount; }

int Vault::addLock(Lock* lock) {
    if (lock == nullptr || count >= 20) return -1;
    for (int i = 0; i < count; ++i)
        if (locks[i] == lock) return -1;
    if (count == cap) grow();
    locks[count] = lock;
    lock->addRef();
    return count++;
}

int Vault::addLock(const char* n, char type, u32 p1, u32 p2, int baseTime) {
    if (count >= 20) return -1;
    Lock* l = new Lock(n, type, p1, p2, baseTime);
    return addLock(l);
}

bool Vault::removeLock(int index) {
    if (index < 0 || index >= count) return false;
    Lock* l = locks[index];
    for (int i = index; i < count - 1; ++i) locks[i] = locks[i + 1];
    --count;
    if (l->release()) delete l;
    return true;
}

int Vault::findLock(const char* n) const {
    for (int i = 0; i < count; ++i)
        if (std::strcmp(locks[i]->getName(), n) == 0) return i;
    return -1;
}

int Vault::crackedCount() const {
    int c = 0;
    for (int i = 0; i < count; ++i)
        if (locks[i]->isCracked()) ++c;
    return c;
}

const Lock* Vault::getLock(int index) const {
    if (index < 0 || index >= count) return nullptr;
    return locks[index];
}

Lock* Vault::getLock(int index) {
    if (index < 0 || index >= count) return nullptr;
    return locks[index];
}

bool Vault::isOpen() const {
    return crackedCount() == count;
}

u32 Vault::key() const {
    if (!isOpen()) return 0;
    u32 k = 0;
    for (int i = 0; i < count; ++i)
        k = ((k << 5) | (k >> 27)) ^ locks[i]->getCode();
    return k;
}

int Vault::noise() const {
    u32 k = key();
    int bits = 0;
    while (k != 0) {
        bits += (int)(k & 1u);
        k >>= 1;
    }
    return bits;
}

u32 Vault::loot() const {
    u64 mod = modVal;
    u64 result = 1 % mod;
    u64 b = baseVal % mod;
    u64 e = expVal;
    while (e > 0) {
        if (e & 1) result = (result * b) % mod;
        b = (b * b) % mod;
        e >>= 1;
    }
    return (u32)result;
}


// Crew

class Crew {
public:
    Crew(const char* name);
    Crew(const Crew& other);
    ~Crew();
    const char* getName() const;
    int memberCount() const;
    int addMember(const Member& member);     // stores its own copy
    int addMember(const char* name, int skill);
    const Member* getMember(int index) const;
    Member* getMember(int index);
    int findMember(const char* name) const;

    
    int estimate(const Vault& vault) const;

    // Only called on a crew with at least one member.
    int execute(Vault& vault);
    static int live();
private:
    Crew& operator=(const Crew&);
    void grow();
    int* openLocksSorted(const Vault& vault, int& n) const;
    int  runJobs(const Vault& vault, Member** ms, const int* order, int n) const;

    char* name;
    Member** members;
    int      count;
    int      cap;
    static int liveCount;
};

int Crew::liveCount = 0;

Crew::Crew(const char* n)
    : name(0), members(0), count(0), cap(2) {
    name = new char[std::strlen(n) + 1];
    std::strcpy(name, n);
    members = new Member * [cap];
    ++liveCount;
}

Crew::Crew(const Crew& other)
    : name(0), members(0), count(other.count), cap(other.cap) {
    name = new char[std::strlen(other.name) + 1];
    std::strcpy(name, other.name);
    members = new Member * [cap];
    for (int i = 0; i < count; ++i)
        members[i] = new Member(*other.members[i]);
    ++liveCount;
}

Crew::~Crew() {
    for (int i = 0; i < count; ++i) delete members[i];
    delete[] members;
    delete[] name;
    --liveCount;
}

void Crew::grow() {
    int newCap = cap * 2;
    Member** bigger = new Member * [newCap];
    for (int i = 0; i < count; ++i) bigger[i] = members[i];
    delete[] members;
    members = bigger;
    cap = newCap;
}

const char* Crew::getName() const { return name; }
int Crew::memberCount() const { return count; }
int Crew::live() { return liveCount; }

int Crew::addMember(const Member& member) {
    if (count >= 20) return -1;
    if (count == cap) grow();
    members[count] = new Member(member);
    return count++;
}

int Crew::addMember(const char* n, int skill) {
    if (count >= 20) return -1;
    if (count == cap) grow();
    members[count] = new Member(n, skill);
    return count++;
}

const Member* Crew::getMember(int index) const {
    if (index < 0 || index >= count) return nullptr;
    return members[index];
}

Member* Crew::getMember(int index) {
    if (index < 0 || index >= count) return nullptr;
    return members[index];
}

int Crew::findMember(const char* n) const {
    for (int i = 0; i < count; ++i)
        if (std::strcmp(members[i]->getName(), n) == 0) return i;
    return -1;
}

// Indices of the vault's uncracked locks, largest base time first
// (equal base times keep vault order). Caller deletes the array.
int* Crew::openLocksSorted(const Vault& vault, int& n) const {
    int total = vault.lockCount();
    int* order = new int[total];
    n = 0;
    for (int i = 0; i < total; ++i) {
        const Lock* l = vault.getLock(i);
        if (l->isCracked()) continue;
        int key = i;
        int j = n - 1;
        while (j >= 0 &&
            vault.getLock(order[j])->getBaseTime() < l->getBaseTime()) {
            order[j + 1] = order[j];
            --j;
        }
        order[j + 1] = key;
        ++n;
    }
    return order;
}


int Crew::runJobs(const Vault& vault, Member** ms, const int* order, int n) const {
    int penalty = (count > 3) ? 1 : 0;
    int* finish = new int[count];
    for (int i = 0; i < count; ++i) finish[i] = 0;

    for (int k = 0; k < n; ++k) {
        const Lock* lock = vault.getLock(order[k]);
        int best = -1;
        int bestCand = 0;
        for (int i = 0; i < count; ++i) {
            int cand = finish[i] + ms[i]->timeFor(*lock) + penalty;
            if (best == -1 || cand < bestCand) {
                best = i;
                bestCand = cand;
            }
        }
        finish[best] = bestCand;
        ms[best]->work(*lock);
    }

    int longest = 0;
    for (int i = 0; i < count; ++i)
        if (finish[i] > longest) longest = finish[i];
    delete[] finish;
    return longest > 1 ? longest : 1;
}

int Crew::estimate(const Vault& vault) const {
    if (count == 0) return -1;
    Member** temp = new Member * [count];
    for (int i = 0; i < count; ++i) temp[i] = new Member(*members[i]);

    int n = 0;
    int* order = openLocksSorted(vault, n);
    int d = runJobs(vault, temp, order, n);

    delete[] order;
    for (int i = 0; i < count; ++i) delete temp[i];
    delete[] temp;
    return d;
}

int Crew::execute(Vault& vault) {
    int n = 0;
    int* order = openLocksSorted(vault, n);
    int d = runJobs(vault, members, order, n);
    for (int k = 0; k < n; ++k) vault.getLock(order[k])->crack();
    delete[] order;
    return d;
}


// Operation

class Operation {
public:
    Operation(const char* codename);
    Operation(const Operation& other);       // the backup plan
    ~Operation();
    const char* getName() const;
    void setName(const char* name);          // stores its own copy
    int addCrew(const Crew& crew);
    int addCrew(const char* name);
    int addVault(const Vault& vault);        // own copy, locks stay shared
    int addVault(const char* name, u32 base, u64 exp, u32 m,
        int windowStart, int windowEnd);
    int crewCount() const;
    int vaultCount() const;
    const Crew* getCrew(int index) const;
    Crew* getCrew(int index);
    const Vault* getVault(int index) const;
    Vault* getVault(int index);
    int findCrew(const char* name) const;
    int findVault(const char* name) const;
    void printPlan() const;                  // changes nothing


    u64 execute();
    bool isDone() const;
    u64 totalLoot() const;                   // 0 before the first execute
    static int getHeat();

    
    static void addHeat(int delta);
    static void resetHeat();                 // heat = 0
    static int live();
private:
    Operation& operator=(const Operation&);
    void growCrews();
    void growVaults();
    void computePlan(int* owner, int* pStart, int* pEnd) const;
    void vaultsOfCrew(int crew, const int* owner, const int* pStart,
        int* idx, int& k) const;

    char* name;
    Crew** crews;
    int      crewN;
    int      crewCap;
    Vault** vaults;
    int      vaultN;
    int      vaultCap;
    bool     done;
    u64      lootTotal;
    static int heat;
    static int liveCount;
};

int Operation::heat = 0;
int Operation::liveCount = 0;

Operation::Operation(const char* codename)
    : name(0), crews(0), crewN(0), crewCap(2), vaults(0), vaultN(0),
    vaultCap(2), done(false), lootTotal(0) {
    name = new char[std::strlen(codename) + 1];
    std::strcpy(name, codename);
    crews = new Crew * [crewCap];
    vaults = new Vault * [vaultCap];
    ++liveCount;
}

Operation::Operation(const Operation& other)
    : name(0), crews(0), crewN(other.crewN), crewCap(other.crewCap),
    vaults(0), vaultN(other.vaultN), vaultCap(other.vaultCap),
    done(false), lootTotal(0) {
    name = new char[std::strlen(other.name) + 1];
    std::strcpy(name, other.name);
    crews = new Crew * [crewCap];
    for (int i = 0; i < crewN; ++i) crews[i] = new Crew(*other.crews[i]);
    vaults = new Vault * [vaultCap];
    for (int i = 0; i < vaultN; ++i) vaults[i] = new Vault(*other.vaults[i]);
    ++liveCount;
    addHeat(3);
}

Operation::~Operation() {
    for (int i = 0; i < crewN; ++i) delete crews[i];
    delete[] crews;
    for (int i = 0; i < vaultN; ++i) delete vaults[i];
    delete[] vaults;
    delete[] name;
    --liveCount;
    addHeat(-2);
}

void Operation::growCrews() {
    int newCap = crewCap * 2;
    Crew** bigger = new Crew * [newCap];
    for (int i = 0; i < crewN; ++i) bigger[i] = crews[i];
    delete[] crews;
    crews = bigger;
    crewCap = newCap;
}

void Operation::growVaults() {
    int newCap = vaultCap * 2;
    Vault** bigger = new Vault * [newCap];
    for (int i = 0; i < vaultN; ++i) bigger[i] = vaults[i];
    delete[] vaults;
    vaults = bigger;
    vaultCap = newCap;
}

const char* Operation::getName() const { return name; }

void Operation::setName(const char* n) {
    char* fresh = new char[std::strlen(n) + 1];
    std::strcpy(fresh, n);
    delete[] name;
    name = fresh;
}

int Operation::addCrew(const Crew& crew) {
    if (crewN >= 20) return -1;
    if (crewN == crewCap) growCrews();
    crews[crewN] = new Crew(crew);
    return crewN++;
}

int Operation::addCrew(const char* n) {
    if (crewN >= 20) return -1;
    if (crewN == crewCap) growCrews();
    crews[crewN] = new Crew(n);
    return crewN++;
}

int Operation::addVault(const Vault& vault) {
    if (vaultN >= 50) return -1;
    if (vaultN == vaultCap) growVaults();
    vaults[vaultN] = new Vault(vault);
    return vaultN++;
}

int Operation::addVault(const char* n, u32 base, u64 exp, u32 m,
    int windowStart, int windowEnd) {
    if (vaultN >= 50) return -1;
    if (vaultN == vaultCap) growVaults();
    vaults[vaultN] = new Vault(n, base, exp, m, windowStart, windowEnd);
    return vaultN++;
}

int Operation::crewCount() const { return crewN; }
int Operation::vaultCount() const { return vaultN; }

const Crew* Operation::getCrew(int index) const {
    if (index < 0 || index >= crewN) return nullptr;
    return crews[index];
}

Crew* Operation::getCrew(int index) {
    if (index < 0 || index >= crewN) return nullptr;
    return crews[index];
}

const Vault* Operation::getVault(int index) const {
    if (index < 0 || index >= vaultN) return nullptr;
    return vaults[index];
}

Vault* Operation::getVault(int index) {
    if (index < 0 || index >= vaultN) return nullptr;
    return vaults[index];
}

int Operation::findCrew(const char* n) const {
    for (int i = 0; i < crewN; ++i)
        if (std::strcmp(crews[i]->getName(), n) == 0) return i;
    return -1;
}

int Operation::findVault(const char* n) const {
    for (int i = 0; i < vaultN; ++i)
        if (std::strcmp(vaults[i]->getName(), n) == 0) return i;
    return -1;
}

bool Operation::isDone() const { return done; }
u64  Operation::totalLoot() const { return lootTotal; }
int  Operation::getHeat() { return heat; }
int  Operation::live() { return liveCount; }

void Operation::addHeat(int delta) {
    heat += delta;
    if (heat < 0) heat = 0;
    if (heat > 100) heat = 100;
}

void Operation::resetHeat() { heat = 0; }

// Greedy per-crew weighted interval scheduling.
// owner[v] = index of the crew given vault v (or -1), pStart/pEnd = its
// planned interval. Arrays have vaultN entries.
void Operation::computePlan(int* owner, int* pStart, int* pEnd) const {
    for (int v = 0; v < vaultN; ++v) {
        owner[v] = -1;
        pStart[v] = 0;
        pEnd[v] = 0;
    }
    int* feas = new int[vaultN];
    int* p = new int[vaultN + 1];
    u64* best = new u64[vaultN + 1];

    for (int c = 0; c < crewN; ++c) {
        if (crews[c]->memberCount() == 0) continue;

        // 1. feasible vaults, sorted by planned end (ties: creation order)
        int n = 0;
        for (int v = 0; v < vaultN; ++v) {
            if (owner[v] != -1) continue;
            int T = crews[c]->estimate(*vaults[v]);
            int s = vaults[v]->windowStart();
            int e = s + T;
            if (e > vaults[v]->windowEnd()) continue;
            pStart[v] = s;
            pEnd[v] = e;
            int j = n - 1;
            while (j >= 0 && pEnd[feas[j]] > e) {
                feas[j + 1] = feas[j];
                --j;
            }
            feas[j + 1] = v;
            ++n;
        }

        // 2. weighted interval scheduling over items 1..n
        best[0] = 0;
        p[0] = 0;
        for (int i = 1; i <= n; ++i) {
            int vi = feas[i - 1];
            p[i] = 0;
            for (int j = i - 1; j >= 1; --j) {
                if (pEnd[feas[j - 1]] <= pStart[vi]) {
                    p[i] = j;
                    break;
                }
            }
            u64 take = (u64)vaults[vi]->loot() + best[p[i]];
            best[i] = take > best[i - 1] ? take : best[i - 1];
        }

        // 3. walk back; on a tie do not take item i
        int i = n;
        while (i > 0) {
            if (best[i] == best[i - 1]) {
                --i;
            }
            else {
                owner[feas[i - 1]] = c;
                i = p[i];
            }
        }
    }

    delete[] best;
    delete[] p;
    delete[] feas;
}

// Collects the vaults given to crew `crew`, in increasing planned start.
void Operation::vaultsOfCrew(int crew, const int* owner, const int* pStart,
    int* idx, int& k) const {
    k = 0;
    for (int v = 0; v < vaultN; ++v) {
        if (owner[v] != crew) continue;
        int j = k - 1;
        while (j >= 0 && pStart[idx[j]] > pStart[v]) {
            idx[j + 1] = idx[j];
            --j;
        }
        idx[j + 1] = v;
        ++k;
    }
}

void Operation::printPlan() const {
    int* owner = new int[vaultN];
    int* pStart = new int[vaultN];
    int* pEnd = new int[vaultN];
    int* idx = new int[vaultN];
    computePlan(owner, pStart, pEnd);

    std::cout << "PLAN " << name << "\n";
    for (int c = 0; c < crewN; ++c) {
        int k = 0;
        vaultsOfCrew(c, owner, pStart, idx, k);
        std::cout << "Crew " << crews[c]->getName() << " -> ";
        if (k == 0) {
            std::cout << "(idle) loot=0\n";
        }
        else {
            u64 total = 0;
            for (int j = 0; j < k; ++j) {
                int v = idx[j];
                if (j > 0) std::cout << " ";
                std::cout << vaults[v]->getName() << "[" << pStart[v] << ","
                    << pEnd[v] << ")";
                total += vaults[v]->loot();
            }
            std::cout << " loot=" << total << "\n";
        }
    }

    std::cout << "Unassigned:";
    bool any = false;
    for (int v = 0; v < vaultN; ++v) {
        if (owner[v] == -1) {
            std::cout << " " << vaults[v]->getName();
            any = true;
        }
    }
    if (!any) std::cout << " none";
    std::cout << "\n";

    delete[] idx;
    delete[] pEnd;
    delete[] pStart;
    delete[] owner;
}

u64 Operation::execute() {
    if (done) return lootTotal;

    int* owner = new int[vaultN];
    int* pStart = new int[vaultN];
    int* pEnd = new int[vaultN];
    int* idx = new int[vaultN];
    computePlan(owner, pStart, pEnd);

    u64 total = 0;
    for (int c = 0; c < crewN; ++c) {
        int k = 0;
        vaultsOfCrew(c, owner, pStart, idx, k);
        int busyUntil = 0;
        for (int j = 0; j < k; ++j) {
            Vault* v = vaults[idx[j]];

            // 1. heat exactly 100: abort this vault, change nothing
            if (heat == 100) {
                std::cout << "[" << crews[c]->getName() << "] " << v->getName()
                    << " ABORT heat=" << heat << "\n";
                continue;
            }

            // 2-3. start time and alert
            int start = v->windowStart() > busyUntil ? v->windowStart() : busyUntil;
            bool alert = heat >= 60;

            // 4-5. do the job
            int T = crews[c]->execute(*v);
            if (alert) T = (int)((3LL * T + 1) / 2);

            // 6. times
            int finish = start + T;
            busyUntil = finish;

            // 7. collected or blown
            bool ok = finish <= v->windowEnd();
            if (ok) {
                total += v->loot();
                addHeat(v->noise() / 8);
            }
            else {
                addHeat(10);
            }

            // 8. print
            std::cout << "[" << crews[c]->getName() << "] " << v->getName()
                << " start=" << start << " finish=" << finish
                << (ok ? " OK" : " BLOWN") << " heat=" << heat << "\n";
        }
    }

    done = true;
    lootTotal = total;
    std::cout << "TOTAL LOOT " << total << "\n";
    std::cout << "HEAT " << heat << "\n";

    delete[] idx;
    delete[] pEnd;
    delete[] pStart;
    delete[] owner;
    return lootTotal;
}


// Lock::crack (needs Operation::addHeat)

u32 Lock::crack() {
    if (cracked) return code;

    if (type == 'M') {
        // modular inverse of p1 modulo p2 (extended Euclid)
        long long r0 = (long long)p2, r1 = (long long)p1;
        long long t0 = 0, t1 = 1;
        while (r1 != 0) {
            long long q = r0 / r1;
            long long tmp = r0 - q * r1;
            r0 = r1;
            r1 = tmp;
            tmp = t0 - q * t1;
            t0 = t1;
            t1 = tmp;
        }
        if (t0 < 0) t0 += (long long)p2;
        code = (u32)t0;
    }
    else if (type == 'G') {
        // undo c ^ (c >> 1): prefix XOR
        u32 c = p1;
        c ^= c >> 1;
        c ^= c >> 2;
        c ^= c >> 4;
        c ^= c >> 8;
        c ^= c >> 16;
        code = c;
    }
    else {
        // type 'X': undo p2 rounds
        u32 x = p1;
        for (u32 r = 0; r < p2; ++r) x = unscrambleRound(x);
        code = x;
    }

    cracked = true;
    bool hardened = (type == 'X' && p2 >= 50000);
    Operation::addHeat(hardened ? 3 : 1);
    return code;
}