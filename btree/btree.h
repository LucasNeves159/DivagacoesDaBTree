#define MAX_CHAVES 204
// Todos os campos devem ter este tamanho
#define TAM_PAG 4096
// Espaço máximo utilizavel 
// (depois será subtraído pelo utilizado)
#define bytes ( TAM_PAG - (MAX_CHAVES * 4) + (MAX_CHAVES * 8) + ((MAX_CHAVES + 1) * 8) + 3)
#define livre (TAM_PAG - bytes)
#define INICIALIZA(x) memset(x, -1, (MAX_CHAVES + 1)*sizeof(short))
#define NOME_ARQUIVO "registros.dat"
#define NOME_INDEX "indices.dat"
#define TRUE 1
#define FALSE 0
#define ORDEM MAX_CHAVES - 1
#define MAX 100
#define TAMANHO_REG (sizeof(int) + (sizeof(char) * 100) + sizeof(float))

// struct que recolhe info no registro.dat
typedef struct dados{
    int nUSP;
    char nome[15];
    char sobrenome[35];
    char curso[50];
    float nota;
} dados;

/* 
 *pega info do nó e busca no registro.dat
 * ou adiciona info em um nó
 */
typedef struct RRN{
    int chave;
    long int endereco;
} RRN;

// busca ou insere no indices.dat
typedef struct No_Pagina{
    short nChaves;    			// quantidade de chaves armazenadas
    RRN *registros[MAX_CHAVES]		// ponteiro de structs com os pares chave-offset
    int *filhos[MAX_CHAVES+1];		// lista de inteiras que representa os filhos de cada chave
    int folha;				// eh folha?
} pagina;

/*
 * Modelo imaginado:
 * main(){
 * 	...
 *
 *	case "inserir":
 * 		scanf(...);
 *		...
 * 		insere(dados); //opera com ambos arquivos
 * 	case "buscar":
 * 		scanf(chave...);
 * 		busca(chave); //opera com ambos arquivos
 * 	case "atualizar":
 * 		scanf(...);
 * 		...
 * 		atualiza(dados); //opera com ambos arquivos
 * }
 *
 * int insere(...){
 * 	if (reg.dat == NULL){
 * 		createTree();
 *		createReg();
 *	} else {
 *		fopen ... 
 *		insertReg(...);
 *		fclose ...
 *		fopen ...
 *		insertBTreeKey(...);
 *		fclose
 *	}
 * }
 *
 * int insertBTreeKey(...){
 * 	(inicializa structs)
 * 	folha_certa = searchLeaf(fp, rrn); //aqui dentro tem o getHeader e no fim retorna uma página
 * 	if (folha lotada){
 * 		folha_certa = maintenceBTree(fp, rrn); //aqui dentro tem o promote e lida com mudança de raiz
 * 	}
 * 		insertIntoLeaf(fp, folha_certa);
 * }
 *
 * pagina *maintenceBTree(...){
 * 	(muitos roles com if(...), splitPage(...), promoteKey(...), promoteRootUpdate(...))
 * }
 *
 * int insertToLeaf(...){
 *	(inicializa nova struct, coloca o chave-offset lá e substitui no arquivo)
 *	...
 * }
 *
 */

// relativo à manipulação geral (elas que vão abrir e fechar os arquivos)
int busca(int);				// função genérica de busca que retorna sucesso ou erro
int insere(dados *);			// função que insere uma entrada, cria os arquivos se necessário
int atualiza(dados *);			// função que modifica um registro

// relativo à btree
FILE *createTree();						// cria um arquivo de btree e uma raiz (O offset inicial é 4096)
void writeHeader(FILE *, long);					// escreve RRN da raiz no header
long getHeader(FILE *);						// vê onde começa a árvore
RRN *getBTreeKey(FILE *, int);					// busca chave na árvore e retorna o par chave-offset correspondente
int insertBTreeKey(FILE *, RRN *);				// chama funções para procurar, splitar, inserir chave-offset em uma fola
pagina *searchLeaf(FILE *, RRN *);				// pega folha pra inserir o par chave-offset
pagina *maintenceBTree(FILE *, pagina *);			// chama split, chama promoteKey, chama promoteRootUpdate, 
								//retorna a folha correta de inserção
promotedKey *splitPage(FILE *, pagina *);			// separa a pagina em duas, atualiza a anterior e cria uma nova, retorna o offset dela
promotedKey *promoteKey(pagina *);				// retorna a chave promovida de uma pagina e remove dela
promotedKey *promoteInsertion(FILE *, pagina*, promotedKey*);	// promoção recursiva, porque merdas podem acontecer
promotedKey *promoteRootUpdate(FILE *, promotedKey*);		// caso aumente a altura da BTree, deve-se fazer as alterações da raiz
int insertIntoLeaf(FILE *, pagina *);				// função responsável por inserir


// relativo ao registro
FILE *createReg.dat
int printReg(FILE *,long);			// imprime o registro achado na BTree
int updateReg(FILE *, dados *, long);		// modifica um registro
long insertReg(FILE *, dados *);		// append e retorna o offset, não tem nem o que discutir.
