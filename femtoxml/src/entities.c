/* FemtoXML - An easy to use XML library
 * 
 * See doc/license.txt for license information (GPLv3)
 * 
 * Authors: 
 * 	Fredrik Hultin, noname@ the domain two lines below, 2007
 *
 * http://nurd.se/~noname/femtoxml
 */

/* entities.c
 *
 * This file contains XML entities, character escaping */

#include "femtoxml.h"
#include "femtoxmli.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static const char* entities[][2] = {
	{"quot",	"\""},
	{"amp",	"&"},
	{"apos",	"'"},
	{"lt",	"<"},
	{"gt",	">"}

/* Not supported by XML - Perhaps they should be parsed anyway to be 'nice'? */
/*	{"nbsp",	" "},
	{"iexcl",	"¡"},
	{"cent",	"¢"},
	{"pound",	"£"},
	{"curren",	"¤"},
	{"yen",	"¥"},
	{"brvbar",	"¦"},
	{"sect",	"§"},
	{"uml",	"¨"},
	{"copy",	"©"},
	{"ordf",	"ª"},
	{"laquo",	"«"},
	{"not",	"¬"},
	{"shy",	" "},
	{"reg",	"®"},
	{"macr",	"¯"},
	{"deg",	"°"},
	{"plusmn",	"±"},
	{"sup2",	"²"},
	{"sup3",	"³"},
	{"acute",	"´"},
	{"micro",	"µ"},
	{"para",	"¶"},
	{"middot",	"·"},
	{"cedil",	"¸"},
	{"sup1",	"¹"},
	{"ordm",	"º"},
	{"raquo",	"»"},
	{"frac14",	"¼"},
	{"frac12",	"½"},
	{"frac34",	"¾"},
	{"iquest",	"¿"},
	{"Agrave",	"À"},
	{"Aacute",	"Á"},
	{"Acirc",	"Â"},
	{"Atilde",	"Ã"},
	{"Auml",	"Ä"},
	{"Aring",	"Å"},
	{"AElig",	"Æ"},
	{"Ccedil",	"Ç"},
	{"Egrave",	"È"},
	{"Eacute",	"É"},
	{"Ecirc",	"Ê"},
	{"Euml",	"Ë"},
	{"Igrave",	"Ì"},
	{"Iacute",	"Í"},
	{"Icirc",	"Î"},
	{"Iuml",	"Ï"},
	{"ETH",	"Ð"},
	{"Ntilde",	"Ñ"},
	{"Ograve",	"Ò"},
	{"Oacute",	"Ó"},
	{"Ocirc",	"Ô"},
	{"Otilde",	"Õ"},
	{"Ouml",	"Ö"},
	{"times",	"×"},
	{"Oslash",	"Ø"},
	{"Ugrave",	"Ù"},
	{"Uacute",	"Ú"},
	{"Ucirc",	"Û"},
	{"Uuml",	"Ü"},
	{"Yacute",	"Ý"},
	{"THORN",	"Þ"},
	{"szlig",	"ß"},
	{"agrave",	"à"},
	{"aacute",	"á"},
	{"acirc",	"â"},
	{"atilde",	"ã"},
	{"auml",	"ä"},
	{"aring",	"å"},
	{"aelig",	"æ"},
	{"ccedil",	"ç"},
	{"egrave",	"è"},
	{"eacute",	"é"},
	{"ecirc",	"ê"},
	{"euml",	"ë"},
	{"igrave",	"ì"},
	{"iacute",	"í"},
	{"icirc",	"î"},
	{"iuml",	"ï"},
	{"eth",	"ð"},
	{"ntilde",	"ñ"},
	{"ograve",	"ò"},
	{"oacute",	"ó"},
	{"ocirc",	"ô"},
	{"otilde",	"õ"},
	{"ouml",	"ö"},
	{"divide",	"÷"},
	{"oslash",	"ø"},
	{"ugrave",	"ù"},
	{"uacute",	"ú"},
	{"ucirc",	"û"},
	{"uuml",	"ü"},
	{"yacute",	"ý"},
	{"thorn",	"þ"},
	{"yuml",	"ÿ"},
	{"OElig",	"Œ"},
	{"oelig",	"œ"},
	{"Scaron",	"Š"},
	{"scaron",	"š"},
	{"Yuml",	"Ÿ"},
	{"fnof",	"ƒ"},
	{"circ",	"ˆ"},
	{"tilde",	"˜"},
	{"Alpha",	"Α"},
	{"Beta",	"Β"},
	{"Gamma",	"Γ"},
	{"Delta",	"Δ"},
	{"Epsilon",	"Ε"},
	{"Zeta",	"Ζ"},
	{"Eta",	"Η"},
	{"Theta",	"Θ"},
	{"Iota",	"Ι"},
	{"Kappa",	"Κ"},
	{"Lambda",	"Λ"},
	{"Mu",	"Μ"},
	{"Nu",	"Ν"},
	{"Xi",	"Ξ"},
	{"Omicron",	"Ο"},
	{"Pi",	"Π"},
	{"Rho",	"Ρ"},
	{"Sigma",	"Σ"},
	{"Tau",	"Τ"},
	{"Upsilon",	"Υ"},
	{"Phi",	"Φ"},
	{"Chi",	"Χ"},
	{"Psi",	"Ψ"},
	{"Omega",	"Ω"},
	{"alpha",	"α"},
	{"beta",	"β"},
	{"gamma",	"γ"},
	{"delta",	"δ"},
	{"epsilon",	"ε"},
	{"zeta",	"ζ"},
	{"eta",	"η"},
	{"theta",	"θ"},
	{"iota",	"ι"},
	{"kappa",	"κ"},
	{"lambda",	"λ"},
	{"mu",	"μ"},
	{"nu",	"ν"},
	{"xi",	"ξ"},
	{"omicron",	"ο"},
	{"pi",	"π"},
	{"rho",	"ρ"},
	{"sigmaf",	"ς"},
	{"sigma",	"σ"},
	{"tau",	"τ"},
	{"upsilon",	"υ"},
	{"phi",	"φ"},
	{"chi",	"χ"},
	{"psi",	"ψ"},
	{"omega",	"ω"},
	{"thetasym",	"ϑ"},
	{"upsih",	"ϒ"},
	{"piv",	"ϖ"},
	{"ensp",	" "},
	{"emsp",	" "},
	{"thinsp",	" "},
	{"zwnj",	" "},
	{"zwj",	" "},
	{"lrm",	" "},
	{"rlm",	" "},
	{"ndash",	"–"},
	{"mdash",	"—"},
	{"lsquo",	"‘"},
	{"rsquo",	"’"},
	{"sbquo",	"‚"},
	{"ldquo",	"“"},
	{"rdquo",	"”"},
	{"bdquo",	"„"},
	{"dagger",	"†"},
	{"Dagger",	"‡"},
	{"bull",	"•"},
	{"hellip",	"…"},
	{"permil",	"‰"},
	{"prime",	"′"},
	{"Prime",	"″"},
	{"lsaquo",	"‹"},
	{"rsaquo",	"›"},
	{"oline",	"‾"},
	{"frasl",	"⁄"},
	{"euro",	"€"},
	{"image",	"ℑ"},
	{"weierp",	"℘"},
	{"real",	"ℜ"},
	{"trade",	"™"},
	{"alefsym",	"ℵ"},
	{"larr",	"←"},
	{"uarr",	"↑"},
	{"rarr",	"→"},
	{"darr",	"↓"},
	{"harr",	"↔"},
	{"crarr",	"↵"},
	{"lArr",	"⇐"},
	{"uArr",	"⇑"},
	{"rArr",	"⇒"},
	{"dArr",	"⇓"},
	{"hArr",	"⇔"},
	{"forall",	"∀"},
	{"part",	"∂"},
	{"exist",	"∃"},
	{"empty",	"∅"},
	{"nabla",	"∇"},
	{"isin",	"∈"},
	{"notin",	"∉"},
	{"ni",	"∋"},
	{"prod",	"∏"},
	{"sum",	"∑"},
	{"minus",	"−"},
	{"lowast",	"∗"},
	{"radic",	"√"},
	{"prop",	"∝"},
	{"infin",	"∞"},
	{"ang",	"∠"},
	{"and",	"∧"},
	{"or",	"∨"},
	{"cap",	"∩"},
	{"cup",	"∪"},
	{"int",	"∫"},
	{"there4",	"∴"},
	{"sim",	"∼"},
	{"cong",	"≅"},
	{"asymp",	"≈"},
	{"ne",	"≠"},
	{"equiv",	"≡"},
	{"le",	"≤"},
	{"ge",	"≥"},
	{"sub",	"⊂"},
	{"sup",	"⊃"},
	{"nsub",	"⊄"},
	{"sube",	"⊆"},
	{"supe",	"⊇"},
	{"oplus",	"⊕"},
	{"otimes",	"⊗"},
	{"perp",	"⊥"},
	{"sdot",	"⋅"},
	{"lceil",	"⌈"},
	{"rceil",	"⌉"},
	{"lfloor",	"⌊"},
	{"rfloor",	"⌋"},
	{"lang",	"〈"},
	{"rang",	"〉"},
	{"loz",	"◊"},
	{"spades",	"♠"},
	{"clubs",	"♣"},
	{"hearts",	"♥"},
	{"diams",	"♦"}*/
};

int fxml_getBits(unsigned int val)
{
	int i;
	for(i = sizeof(int) * 8; i >= 0; i--){
		if((val >> i) & 1){
			return i + 1;
		}
	}
	return 0;
}

typedef unsigned char byte;

void UR_HexDump(const char* name, const void* what, int size)
{
        int i;
        printf("\n%s\n", name);
        for(i = 0; i < size; i++){
                if(i % 16 == 0){ printf("\n"); }
                printf("0x%02x ", ((const unsigned char*)what)[i]);
        }
        printf("\n");
        
	for(i = 0; i < size; i++){
		int j;
		for(j = 7; j >= 0; j--){
                	printf("%d", ((((const unsigned char*)what)[i]) >> j) & 1);
		}
		printf(" ");
        }

        printf("\n");
}

byte* fxml_encodeUtf8(int codepoint, byte* out)
{
	int i, j, bits, atBit = 0, leftInZero = 6;

	/* Pure ASCII -> return it as is */
	if(codepoint < 128){
		out[0] = (byte)codepoint;
	}

	/* nngh, have to encode utf-8 >_> */

	bits = FXML_MIN(fxml_getBits(codepoint), 31);

	/* start with the highest byte and fill it with bits */
	for(i = 5; i >= 0; i--){
		/*printf("%d atBit\n", atBit);
		printf("%d %d %d %d\n", bits, atBit, bits - atBit, leftInZero);*/

		if(bits - atBit <= leftInZero){
			/* We can fit this in byte zero, filler up! */

			/* "number of bytes" with 1s, then a 0 (already 0) */
			for(j = 7; j >= 7 - (5 - i); j--){
				out[i] |= 1 << j;
			}

			/* fill up with the rest of the bits */
			for(j = 0; atBit < bits; j++){
				printf("j: %d\n", j);
				out[i] |= ((codepoint >> atBit++) & 1) << j;
			}

			/*UR_HexDump("the char", (const void*)(out + i), strlen((const char*)(out + i)));
			printf("%s\n", (char*)(out + i));*/
			return out + i;
		}

		out[i] = 0x80; /* Initialize to 10xxxxxx */
		for(j = 0; j < 6; j++){
			/* Fill with bits from the codepoint to bit 0 -> 5 */
			out[i] |= ((codepoint >> atBit++) & 1) << j;
		}
		leftInZero--;
	}

	/* should not get here */
	return NULL;
}

const char* fxml_getCharFromEntity(const char* entity, int len, byte* cpDec)
{
	int i;
	
	if(len > 1 && entity[0] == '#'){
		unsigned int codepoint = 0;
		int ret;
		memset(cpDec, 0, 7);

		ret = sscanf(entity, entity[1] == 'x' ? "#x%x" : "#%u", &codepoint);

		if(!ret || ret == EOF){
			return NULL;
		}

		printf("%d codepoint\n", codepoint);

		return (char*)fxml_encodeUtf8(codepoint, cpDec);
	}


	for(i = 0; i < /*253*/ 5; i++){
		if(fxml_strncmp(len, entity, entities[i][0])){
			return entities[i][1];
		}
	}

	return NULL;
} 

int fxml_getEntityName(fxml_vFile* f, char* entname, int len)
{
	int ei = 0, i = 0;
	char read;
	memset(entname, 0, 32);	

	while((read = fxml_vfgetc(f)) != ';'){
		entname[ei++] = read;
		if(i >= len - 1 || ei >= 32 - 1){
			return 0;
		}
		i++;
	}

	return ei;
}

int fxml_decodeToString(fxml_vFile* f, char* buffer, int len)
{
	int i, bi = 0;
	for(i = 0; i < len; i++){
		int read = fxml_vfgetc(f);

		if(read > 31 && read != '&'){
			if(buffer){ buffer[bi] = read; }
			bi++;
		}else if(read == '&'){
			char entname[32];
			long pos = fxml_vftell(f);
			const char* ent;
			char entDecodedCodePoint[7];
			int entLen, entNameLen = fxml_getEntityName(f, entname, len - i);

			if(!entNameLen || !(ent = fxml_getCharFromEntity(entname, entNameLen, (unsigned char*)entDecodedCodePoint))){
				fxml_filePos position;

				fxml_vfseek(f, pos, SEEK_SET);
				
				if(buffer){ buffer[bi] = '&'; };
				bi++;

				if(buffer){
					position = fxml_getFilePos(f);
					fxml_ePrint(FXML_MSG_WARNING, "Invalid entity (line: %d column: %d)", position.line, position.col);
				}

				continue;
			}

			/*printf("entity with name: %s -> %s\n", entname, ent);*/

			entLen = strlen(ent);
			if(buffer){ strncpy(buffer + bi, ent, entLen); }
			bi += entLen;
			i += entNameLen + 1;
		}else{
			if(buffer){ buffer[bi] = ' '; }
			bi++;
		}
	}

	if(buffer){ buffer[bi] = '\0'; }
	return bi + 1;
}

void fxml_fsetValEntityDecode(fxml_element* element, fxml_vFile* f, int len)
{
	long pos = fxml_vftell(f);
	int size = fxml_decodeToString(f, NULL, len);
	element->val = realloc(element->val, size);
	fxml_vfseek(f, pos, SEEK_SET);
	fxml_decodeToString(f, element->val, len);
}
