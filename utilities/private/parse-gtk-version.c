#include <stdio.h>
#include <gtk/gtkversion.h>

/* I'm not sure who would try to compile this program under GTK, but it's just
 * a sanity check.
 */
#ifndef IS_STLWRT
 #error "This program must be compiled under STLWRT."
#else

#define	GTK_COMPAT_AS_STRING	##STLWRT_GTK_COMPAT

void	main()
{
	FILE	*makefile = fopen("public-libs/" GTK_COMPAT_AS_STRING
	                          "Makefile.am", "w");
	
	FILE	*gdk_pc = fopen("public-libs/" GTK_COMPAT_AS_STRING
	                                       _GDK_PCONFIG_NAME, "w");
	
	FILE	*gtk_pc = fopen("public-libs/" GTK_COMPAT_AS_STRING
	                                       _GTK_PCONFIG_NAME, "w");
	
	FILE	*gup_pc = fopen("public-libs/" GTK_COMPAT_AS_STRING
	                                       _GUP_PCONFIG_NAME, "w");
	
	
	if (!makefile || !gdk_pc_in || !gtk_pc_in || !gup_pc_in)
	{
		perror("Could not open the output files for writing");
		
		return;
	}
	
	
	/* Output an Automakefile. */
	fprintf(makefile, "gtk_major_version=%d\n", GTK_MAJOR_VERSION);
	fprintf(makefile, "gtk_minor_version=%d\n", GTK_MINOR_VERSION);
	fprintf(makefile, "gtk_micro_version=%d\n", GTK_MICRO_VERSION);
	
	fprintf(makefile, "gtk_binary_age=%d\n", GTK_BINARY_AGE);
	fprintf(makefile, "gtk_interface_age=%d\n", GTK_INTERFACE_AGE);
	
	fprintf(makefile, "gtk_library_name=%s\n", _GTK_LIB_NAME);
	
	fprintf(makefile, "gdk_package_config_name=%s\n", _GDK_PCONFIG_NAME);
	fprintf(makefile, "gtk_package_config_name=%s\n", _GTK_PCONFIG_NAME);
	fprintf(makefile, "gup_package_config_name=%s\n", _GUP_PCONFIG_NAME);



	/* Now output a pkg-config file for each of the following:
	   GDK, GTK and GTK Unix Print Support (GUP). */
	
	fprintf(gdk_pc, "");
	
	return;
}

#endif /* IS_STLWRT */
