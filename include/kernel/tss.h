#ifndef TSS_H
#define TTS_H


struct tss{

	uint32_t prev_tss;
	uint32_t esp0;
	uint32_t ss0;
	uint32_t esp1;
	uint32_t ss1;
	uint32_t esp2;
	uint32_t ss2;

} __attribute__((packed));

#endif
