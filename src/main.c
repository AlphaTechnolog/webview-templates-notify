#include <stdio.h>
#include <stdlib.h>

struct person_t
{
	const char* name;
	const char* lastname;
};

static void die(const char* prefix);
static struct person_t* person_init(const char* name, const char* lastname);
static void person_greet(struct person_t* p);
static void person_deinit(struct person_t* p);

void die(const char* prefix)
{
	perror(prefix);
	exit(EXIT_FAILURE);
}

struct person_t* person_init(const char* name, const char* lastname)
{
	struct person_t* p;

	if (!(p = malloc(sizeof(struct person_t))))
		die("malloc");

	p->name = name;
	p->lastname = lastname;

	return p;
}

void person_greet(struct person_t* p)
{
	printf("Hello %s %s\n", p->name,
			p->lastname);
}

void person_deinit(struct person_t* p)
{
	free(p);
}

int main(int argc, char *argv[])
{
	struct person_t* p;

	p = person_init("John", "Doe");
	person_greet(p);

	person_deinit(p);

	return 0;
}
