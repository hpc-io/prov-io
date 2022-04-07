#include <unistd.h>
#include "provio.h"
#include <mpi.h>

void function_1(provio_helper_t* helper, prov_fields* fields) {
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

	add_prov_record(helper, fields);

    prov_stat.TOTAL_PROV_OVERHEAD += elapsed;
	func_stat(__func__, elapsed);
}


int main() {

	MPI_Init(NULL, NULL);

	/* Initialize provenance fields and others */
	prov_fields fields;
	provio_init(&fields);


	/* Initialize provenance helper */
	provio_helper_t* helper;
	helper = provio_helper_init(&fields);

	/* Track provenance of IO API function_1 */
	function_1(helper, &fields);

	/* Free provenance helper, serialize provenance to disk
	and print stats */
	provio_helper_teardown(helper, &fields);

	/* Free provenance structures */
	provio_term(&fields);

	return 0;
}
