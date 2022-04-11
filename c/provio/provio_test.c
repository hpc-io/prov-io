#include <unistd.h>
#include "provio.h"
#include <mpi.h>

/* PROV-IO instrument start */
/* PROV-IO instrument end */

// /* statistics */
// Stat prov_stat;
// duration_ht* FUNCTION_FREQUENCY;

void function_1(prov_config* config, provio_helper_t* helper, prov_fields* fields) {
	/* Fill in provenance fields */
	const char* obj_name = "Attr_1";
	const char* type = "provio:Attr";
	const char* io_api = "function_1";
    const char* relation = "prov:wasGeneratedBy";
	prov_fill_data_object(fields, obj_name, type);
	prov_fill_relation(fields, relation);

	unsigned long start = get_time_usec();
	sleep(1);
	unsigned long elapsed = (get_time_usec() - start);

	prov_fill_io_api(fields, io_api, elapsed);

	add_prov_record(config, helper, fields);

    prov_stat.TOTAL_PROV_OVERHEAD += elapsed;
	func_stat(__func__, elapsed);
}


int main() {
	prov_config config;
	prov_fields fields;


	MPI_Init(NULL, NULL);

	/* Initialize provenance fields and others */

	provio_init(&config, &fields);

	/* Initialize provenance helper */
	provio_helper_t* helper;
	helper = provio_helper_init(&config, &fields);

	/* Track provenance of IO API function_1 */
	function_1(&config, helper, &fields);

	/* Free provenance helper, serialize provenance to disk
	and print stats */
	provio_helper_teardown(&config, helper, &fields);

	/* Free provenance structures */
	provio_term(&config, &fields);

    MPI_Finalize();

	return 0;
}
