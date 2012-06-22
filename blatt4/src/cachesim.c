/**
 * Vorlesung Betriebssysteme und Rechnerarchitekturen
 *
 * Prof. Dr. Robert Kaiser
 * Hochschule RheinMain, DCSM
 *
 * @file  cacheseim.c
 * @brief Programm zur Simulation eines vollassoziativen Cache
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

/**
 * Verwaltungsinformationen einer Cachezeile:
 */
struct cacheline {
  /** Tag */
  unsigned int tag;
  /** Flag ob gueltig  */
  unsigned int valid;
  /** Anzahl Hits dieser Zeile */
  unsigned int hits;
  /** Zeitstempel des letzten Zugriffs */
  unsigned int last_use;
  /** Zeitstempel des ersten Zugriffs */
  unsigned int first_use;
};


/**
 * Struktur des simulierten Caches:
 */
struct cache
{
    /** Cachezeilen (werden zur Laufzeit mit malloc() alloziert) */
    struct cacheline *lines;

    /** Ersetzungsstrategie (z.B. LRU, Random, etc) */
    int (*replacement_policy)(struct cacheline *L, unsigned int ways);

    /** Anzahl der Cachezeilen */
    unsigned int num_cache_lines;

    /** Zweier-Logarithmus der Anzahl der Cachezeilen */
    unsigned int ld_num_cache_lines;

    /** Anzahl der Wege */
    unsigned int ways;

    /** Zweier-Logarithmus der Anzahl der Wege */
    unsigned int ld_ways;

    /** Groesse der Cachezeilen */
    unsigned int line_size;

    /** Zweier-Logarithmus der Cachezeilen-Groesse */
    unsigned int ld_line_size;

    /** Anzahl Hits und Misses im gesamten Cache */
    unsigned int hits;
    unsigned int misses;
};


/**
 * Statische Daten: der simulierte Cache und ein ,,Zeitstempel'',
 * der staendig hochgezaehlt wird
 */
static struct cache TheCache;
static unsigned long timestamp;

/** wenn != 0: Erweiterte Ausgaben machen */
static int verbose = 0;


/**
 * Einfacher Pseudo-Zufallszahlengenerator: kann
 * auch anstelle der random()-Funktion des Systems
 * verwendet werden
 */
static unsigned int seed = 1234;

static unsigned int random_number(void)
{
    unsigned int z;
    z = seed;
    z *= 32719;
    z += 3;
    z %= 32749;
    seed = z;
    return(z);
}


/**
 * Es folgen verschiedene Ersetzungsstrategien. Diese
 * werden aufgerufen, wenn findcache() (siehe unten) bei
 * einem Cache Miss keine freie Cachezeile findet
 * Diese erhalten als erstes Argument einen Zeiger auf die
 * erste Cachezeile eines Satzes @param L und als
 * zweites Argument die Anzahl der Zeilen innerhalb des
 * Satzes @param ways. Sie wählen dann jeweils entsprechend
 * ihrer Strategie eine Cache Zeile aus und liefern ihren
 * Index zurück.
 */

/** Random-Ersetzung: liefert eine Zufallszahl im Bereich 0 ... ways - 1 */
static int random_choose(struct cacheline *L, unsigned int ways)
{
    return(random() % ways);
}


/** LFU-Ersetzung: liefert die Cachezeile mit den wenigsten Zugriffen  */
static int LFU_choose(struct cacheline *L, unsigned int ways)
{
    unsigned int least_hits = -1;
    unsigned int i, least_hits_index = 0;
    for(i = 0; i < ways; i++)
    {
        if(L[i].valid && L[i].hits < least_hits)
        {
            least_hits = L[i].hits;
            least_hits_index = i;
        }
    }
    return(least_hits_index);
}

/** LRU-Ersetzung: liefert die Cachezeile auf die am laengsten nicht mehr zugegriffen wurde  */
static int LRU_choose(struct cacheline *L, unsigned int ways)
{
    unsigned int lru = -1;
    unsigned int i, lru_index = 0;
    for(i = 0; i < ways; i++)
    {
        if(L[i].valid && L[i].last_use < lru)
        {
            lru = L[i].last_use;
            lru_index = i;
        }
    }
    return(lru_index);
}

/** FIFO-Ersetzung: liefert die aelteste Cachezeile, d.h. die Zeile, deren erste Nutzung am laengsten zurueckliegt   */
static int FIFO_choose(struct cacheline *L, unsigned int ways)
{
    unsigned int oldest = -1;
    unsigned int i, oldest_index = 0;
    for(i = 0; i < ways; i++)
    {
        if(L[i].valid && L[i].first_use < oldest)
        {
            oldest = L[i].first_use;
            oldest_index = i;
        }
    }
    return(oldest_index);
}



/**
 * Hier wird der Cache-Zugriff simuliert
 */
static int findcache(struct cache *C, unsigned long address)
{
    unsigned int tag, index, i;

    if(verbose)
        printf("Addr: 0x%lx -> ", address);

    /**
     * +---------------------------------+
     * |   Tag    |    Index    | Byteadr|
     * +---------------------------------+
     * <----T ---><----- I ----><---B---->
     *
     * Von den 32 Bit einer Adresse entfallen:
     * -  @param B Bits auf die Byte-/Wortauswahl
     * -  @param I Bits auf den Index
     * -  @param T Bits auf den Tag
     *
     * Dabei ist @param B der Zweierlogarithmus der Zeilengröße,
     * @param I ist der Zweierlogarithmus der Anzahl der Sets
     * @param T ist dann der Rest (Alle drei zusammen müssen
     * 32 ergeben: T+I+B=32)
     */

    /* Hier Ihr Code ...*/
    /**
     * Was ist zu tun?
     * 1) Ermittle aus der Adresse den Tag und den Index des auszuwählenden Sets.
     * 2) Wähle anhand des Index einen Aet aus
     * 3) Suche im Set eine Zeile mit uebereinstimmendem Tag und mit gesetztem valid-Flag
     * 4) Falls einer gefunden wird -> cache hit, sonst cache miss.
     *   a) falls Hit: diverse Zähler inkementieren, 1 zurückliefern
     *   b) falls Miss: Zähler inkrementieren, einen noch unbenutzen Eintrag
     *      (einen mit valid = 0) im Set suchen. Falls sich kein solcher findet:
     *      anhand von @param replacement_policy einen zu verdrängenden Eintrag auswählen
     *   c) Eintrag belegen (valid = 1, tag setzen, etc.
     */

}

/** Hilfsroutine: invalidieren einer Cachezeile */
static void invline(struct cache *C, int l)
{
    C->lines[l].tag = -1;
    C->lines[l].valid = 0;
    C->lines[l].hits = 0;
}

/** Hilfsroutine: invalidieren aller Cachezeilen */
static void invcache(struct cache *C)
{
    unsigned int i;
    for(i = 0; i < C->num_cache_lines; i++)
    {
        invline(C, i);
    }
}

/** Hifsroutine: Zweierlograrithmus */
static int ld(unsigned int x)
{
    unsigned int log;
    /**
     * Zahl x so lange nach rechts schieben, bis
     * dabei 1 herauskommt.
     */
    for(log = 0; log < 32; log++)
    {
        if(1 == (x >> log))
        {
            if(0 != (x & (~(1 << log))))
            {   /** Fehler: es sind noch andere Bits in x gesetzt */
                return(-1);
            }
            return(log);
        }
    }
    return(-1);
}


/** Hilfsroutine: Initialisieren des Cache */
static void initcache(struct cache *C, unsigned int cachesize, unsigned int linesize, int ways)
{
    unsigned int i;

    if(ld(cachesize) == -1)
    {
        printf("Cache Size must be a power of two!\n");
        exit(1);
    }

    C->line_size = linesize;
    if((C->ld_line_size = ld(C->line_size)) == -1)
    {
        printf("Line Size must be a power of two!\n");
        exit(1);
    }
    C->num_cache_lines = cachesize / linesize;
    if((C->ld_num_cache_lines = ld(C->num_cache_lines)) == -1)
    {
        printf("Cache Line Size must be a power of two!\n");
        exit(1);
    }

    if(ways == -1) /** ways = -1 ->vollassoziativer Cache */
    {
        ways = C->num_cache_lines;
    }

    C->ways = ways;
    if((C->ld_ways = ld(C->ways)) == -1)
    {
        printf("Ways must be a power of two!\n");
        exit(1);
    }

    /** Platz fuer Cachezeilen allozieren: */
    C->lines = (struct cacheline*)malloc(C->num_cache_lines * sizeof(struct cacheline));

    invcache(C);
    C->hits = C->misses = 0;
}

static unsigned long linear_address(unsigned int i, unsigned int step, unsigned long maxaddress)
{
    return((i * step) % maxaddress);
}

static unsigned long random_address(unsigned int i, unsigned int step, unsigned long maxaddress)
{
    return(random() % maxaddress);
}

static unsigned long pattern[] =
{
    0x00002091,
    0x00005492,
    0x00002093,
    0x00005494,
    0x0000109A,
    0x0000F49B,
    0x0000109C,
    0x0000F49D
};

static unsigned long pattern_address(unsigned int i, unsigned int step, unsigned long maxaddress)
{
    return(pattern[i % (sizeof(pattern)/sizeof(unsigned long))]);
}


static void rtfm(char* argv[])
{
    printf("Usage: %s {Options}\n", argv[0]);
    printf("Options:\n");
    printf("    -a <size>    Set size of address range to cover\n");
    printf("    -c <size>    Set size of cache\n");
    printf("    -g <seq>     select address sequence, <seq> may be one of:\n");
    printf("                   rand - random\n");
    printf("                   lin  - linear\n");
    printf("                   pat  - predefined pattern\n");
    printf("    -l <size>    Set cache line size\n");
    printf("    -p <policy>  select replacement policy, <policy> may be one of:\n");
    printf("                   rand - random\n");
    printf("                   fifo - FIFO\n");
    printf("                   lru  - least recently used\n");
    printf("                   lfu  - least frequently used\n");
    printf("    -r <runs>    Set number of runs\n");
    printf("    -s <inc>     Set per step address increment\n");
    printf("    -v           Be verbose\n");
    printf("    -w <ways>    Set number of ways in cache (default: fully associative)\n");
}

main(int argc, char *argv[])
{
    unsigned long address, max_address;
    int i, opt;
    unsigned int hits, accesses;
    unsigned int loopstep;
    unsigned int runs;
    unsigned int cachesize;
    unsigned int linesize;
    unsigned int ways;
    unsigned int step, steps;
    int loopstep_given = 0;
    char *end;
    unsigned long (*address_gen)(unsigned int i, unsigned int step, unsigned long maxaddress);


    /** Defaultwerte setzen */
    runs = 20;
    max_address = 2560UL;
    cachesize = 2048UL;
    linesize = 16;
    ways = -1;
    address_gen = linear_address;
    TheCache.replacement_policy = random_choose;

    /**
     * Kommandozeilen-Optionen auswerten: siehe "man getopt"
     */
    while ((opt = getopt(argc, argv, "a:c:g:l:p:r:s:vw:")) != EOF)
    {
        switch (opt)
        {
            case '?':
                rtfm(argv);
                exit(0);
                break;
            case 'a':
                max_address = strtoul(optarg, &end, 0);
                if (end == optarg)
                {
                    rtfm(argv);
                    exit(1);
                }
                break;
            case 'c':
                cachesize = strtoul(optarg, &end, 0);
                if (end == optarg)
                {
                    rtfm(argv);
                    exit(1);
                }
                break;
            case 'g':
                if (!strcmp(optarg, "rand"))
                    address_gen = random_address;
                else if (!strcmp(optarg, "lin"))
                    address_gen = linear_address;
                else if (!strcmp(optarg, "pat"))
                    address_gen = pattern_address;
                else
                {
                    rtfm(argv);
                    exit(1);
                }
                break;
            case 'l':
                linesize = strtoul(optarg, &end, 0);
                if (end == optarg)
                {
                    rtfm(argv);
                    exit(1);
                }
                break;
            case 'p':
                if (!strcmp(optarg, "rand"))
                    TheCache.replacement_policy = random_choose;
                else if (!strcmp(optarg, "lru"))
                    TheCache.replacement_policy = LRU_choose;
                else if (!strcmp(optarg, "lfu"))
                    TheCache.replacement_policy = LFU_choose;
                else if (!strcmp(optarg, "fifo"))
                    TheCache.replacement_policy = FIFO_choose;
                else
                {
                    rtfm(argv);
                    exit(1);
                }
                break;
            case 'r':
                runs = strtoul(optarg, &end, 0);
                if (end == optarg)
                {
                    rtfm(argv);
                    exit(1);
                }
                break;
            case 's':
                loopstep = strtoul(optarg, &end, 0);
                if (end == optarg)
                {
                    rtfm(argv);
                    exit(1);
                }
                loopstep_given = 1;
                break;
            case 'v':
                verbose = 1;
                break;
            case 'w':
                ways = strtoul(optarg, &end, 0);
                if (end == optarg)
                {
                    rtfm(argv);
                    exit(1);
                }
                break;
        }
    }

    initcache(&TheCache, cachesize, linesize, ways);

    /**
     * Falls kein Wert fuer loopstep angegeben wurde -> nimm die Cachezeilengroesse
     */
    if(!loopstep_given)
        loopstep = TheCache.line_size;

    /**
     * Die Simulation: eine Anzahl von Adressen wird im Cache
     * nachgefragt, und die Hits werden gezaehlt
     */



    timestamp = 0;

    /**
     * Falls das vorgegebene Adressmuster ausgewählt wurde,
     * (address_gen == pattern_address):
     * setze @param steps = <Anzahl_Elemente_im_Pattern>
     * Ansonsten setze @param steps = <Groesse_des_Adressbereichs/Schrittweite>
     */
    if(address_gen == pattern_address)
        steps = sizeof(pattern)/sizeof(unsigned long);
    else
        steps = max_address / loopstep;
    for(i = 0; i < runs; i++)
    {
        hits = 0;
        accesses = 0;
        for(step = 0; step < steps; step++)
        {
            address = address_gen(step, loopstep, max_address);
            ++accesses;
            if(findcache(&TheCache, address))
                ++hits;
            ++timestamp;
        }
        printf("Run %d: Hit Rate %d%%\n", i+1, (hits * 1000 + 5) / (10*accesses));
    }
    return(0);
}
