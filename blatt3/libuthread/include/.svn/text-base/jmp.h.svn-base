#ifndef _JMP_H_
#define _JMP_H_

/**
 *  @file       jmp.h
 *  @brief      setjmp/longjmp Implementierung
 *
 * Dies ist eine Einfache Implementierung der C Standardfunktionen
 * @c setjmp() und @c longjmp(). Im Unterschied zu diesen Standardfunktionen
 * erlaubt diese Implementierung zusaetzlich das gezielte Manipulieren
 * des Stackpointers und des Befehlszeigers eines geretteten Registersatzes.
 * Diese Funktionalitaet ist noetig, um neue Threads erzeugen zu koennen.
 *
 */

/** Dies ermoeglicht das Kompilieren dieses Codes auch mit nicht-GNU compilern */
#ifndef __GNUC__
#define __attribute__(x)
#define __inline__
#endif


#if defined( __i386__ )
/**
 * This part is specific to the intel x86 machine architecture
 */

/**
 * @brief Indizes der Elemente des @c __jmp_buf arrays
 *
 * Achtung: Diese Definitionen muessen zum Code der Implementierung passen!
 *          Bitte keinesfalls aendern ohne den Assembler-Code in jmp.c entsprechend anzupassen!
 */
#define JB_BX  0
#define JB_SI  1
#define JB_DI  2
#define JB_BP  3
#define JB_SP  4
#define JB_PC  5
#define JB_SIZE 6


/**
 * @typedef __reg_t
 * @brief  Register-Typ
 *
 * Dieser Datentyp beschreibt die Groesse eines Prozessorregister.
 * Für einen 32-bit-Prozessor sollte sizeof(__reg_t) = 4, für einen
 * 64-bit-Prozessor = 8 sein.
 */
typedef unsigned int __reg_t;

/**
 * @struct __jmp_buf
 * @brief  Registerkontext
 *
 * In dieser Struktur werden die register des Prozessors
 * gespeichert. Fuer alle Architecturen muss __jmp_buf[JB_SP]
 * der Stackpointer und __jmp_buf[JB_PC] der Befehlszaehler des
 * Aufrufers sein.
 */

typedef __reg_t  __jmp_buf[JB_SIZE];

/**
 * @brief Manipulieren des Stackpointers
 *
 *
 * @param env Registerkontext auf dem gearbeitet wird
 * @param sp  Wert, der in den Stackpointer geladen werden soll
 * @retval nichts
 */
static __inline__ void jmp_buf_set_sp(__jmp_buf env, void *sp)
{
    env[JB_SP] = (__reg_t)sp;
}

#elif defined( __x86_64 )
/**
 * das Gleiche fuer AMD 64:
 */

#define JB_PC   0
#define JB_RBX  1
#define JB_SP   2
#define JB_RBP  3
#define JB_R12  4
#define JB_R13  5
#define JB_R14  6
#define JB_R15  7
#define JB_SIZE 8

typedef unsigned long __reg_t;

typedef __reg_t __jmp_buf[JB_SIZE];

/**
 * @brief Manipulieren des Stackpointers
 *
 *
 * @param env Registerkontext auf dem gearbeitet wird
 * @param sp  Wert, der in den Stackpointer geladen werden soll
 * @retval nichts
 */
static __inline__ void jmp_buf_set_sp(__jmp_buf env, void *sp)
{
    /**
     * Die Funktion __longjmp() hat im Falle des AMD 64 die Eigenschaft,
     * den Stackpointer bem Verlassen zu inkrementieren. Um dies zu kompensieren
     * wird hier env[JB_SP] mit einem entsprechend pre-dekrementierten Wert
     * geladen, damit nach __longjmp() der Stackpointer wirklich der gewuenschte 
     * Wert enthalten ist.
     */
    env[JB_SP] = (__reg_t)((__reg_t*)sp - 1);
}

#else

#error "Jmp.h muss an diese Architektur angepasst werden."

#endif

/**
 * @brief Manipulieren des Programmzaehlers
 *
 *
 * @param env Registerkontext auf dem gearbeitet wird
 * @param pc  Wert, der in den Programmzaehler geladen werden soll
 * @retval nichts
 */
static __inline__ void jmp_buf_set_pc(__jmp_buf env, void (*pc)())
{
    env[JB_PC] = (__reg_t)pc;
}



/**
 * @brief Registerkontext abspeichern
 *
 * @param env @c __jmp_buf Struktur in die gespeichert werden soll
 * @retval 0 Wenn direkt zurueckgekehrt wird
 * @retval nichtnull wenn ueber @c longjmp() zurueckgekehrt wird
 */
int __setjmp(__jmp_buf env);

/**
 * @brief Mit @c setjmp() geretteten Registerkontext wiederherstellen
 *
 * @param  env @c __jmp_buf Struktur mit dem herzustellenden Registerkontext
 * @param  val Rueckgabewert fuer @c setjmp()
 * @retval keiner
 */
void __longjmp(__jmp_buf env, int val);


#endif /* _JMP_H_ */
