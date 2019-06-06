#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <alloca.h>
#include <sys/stat.h>

#include "vicmain_c"
#include "zvproto.h"
#include "mp_routines.h"
//#include "dlrmapsub.h"
#include "dtm.h"
#include "gdal.h"
#include "ogr_srs_api.h"
#include "cpl_string.h"

#define MAX_STRING_SIZE 120
#define MSG_STRING_SIZE 80

void main44() {
		
	/*			VARIABLES																		*/

	int			status, count;
	char		msg[MSG_STRING_SIZE];

	char		file_is_dtm[MSG_STRING_SIZE];

	int			out_unit;

	short int *inbuf_half;
	short int *in_line_buf_half;
	float *inbuf_float;

	long int line;

	char		in_name[MAX_STRING_SIZE];
	char		out_name[MAX_STRING_SIZE];

	//MP			mp_dtm_obj;
	//dtm			dtmlabel;
	/*typedef struct  {int dtm_maximum_dn;
	                 int dtm_minimum_dn;
	                 int dtm_missing_dn;
	                 float dtm_offset;
	                 float dtm_scaling_factor;
	                 float dtm_a_axis_radius;
	                 float dtm_b_axis_radius;
	                 float dtm_c_axis_radius;
	                 float dtm_body_long_axis;
	                 char dtm_positive_longitude_direction[5];
	                 char dtm_height_direction[7];
	                 char dtm_desc[250];
	                }dtm;*/


	status = zvp("inp", in_name, &count);
	GDALDatasetH hDataset;
	GDALAllRegister();
	hDataset = GDALOpen ( in_name, GA_ReadOnly );
	if ( hDataset == NULL ) {
		sprintf(msg, "Error opening input file %s!", in_name);
		zvmessage(msg,"");
		zabend();
	}
	GDALRasterBandH hBand;
	hBand = GDALGetRasterBand( hDataset, 1 );

	int nXSize = GDALGetRasterBandXSize( hBand );
	int nYSize = GDALGetRasterBandYSize( hBand );
	/*inbuf   = (short int *) malloc (nXSize*nYSize*sizeof(short int));
	if (inbuf == (short int *)NULL) {
		sprintf(msg, "Error during allocation of input buffer!!");
		zvmessage(msg,"");
		zabend();
	}*/
	inbuf_float   = (float *) malloc (nXSize*nYSize*sizeof(float));
	if (inbuf_float == (float *)NULL) {
		sprintf(msg, "Error during allocation of input buffer!!");
		zvmessage(msg,"");
		zabend();
	}
	inbuf_half   = (short int *) malloc (nXSize*nYSize*sizeof(short int));
		if (inbuf_half == (short int *)NULL) {
			sprintf(msg, "Error during allocation of input buffer!!");
			zvmessage(msg,"");
			zabend();
		}
	in_line_buf_half = (short int *) malloc (nXSize*sizeof(short int));
	if (in_line_buf_half == (short int *)NULL) {
		sprintf(msg, "Error during allocation of input buffer!!");
		zvmessage(msg,"");
		zabend();
	}

	int nBlockXSize, nBlockYSize;
	GDALGetBlockSize( hBand, &nBlockXSize, &nBlockYSize );
	/*printf( "Block=%dx%d Type=%s, ColorInterp=%s\n",
	        nBlockXSize, nBlockYSize,
	        GDALGetDataTypeName(GDALGetRasterDataType(hBand)),
	        GDALGetColorInterpretationName(
	            GDALGetRasterColorInterpretation(hBand)) );*/

	double adfGeoTransform[6];
	if( GDALGetGeoTransform( hDataset, adfGeoTransform ) == CE_None ) {
	    /*printf( "Origin = (%.6f,%.6f)\n",
	            adfGeoTransform[0], adfGeoTransform[3] );
	    printf( "Pixel Size = (%.6f,%.6f)\n",
	            adfGeoTransform[1], adfGeoTransform[5] );*/
	}
	//double sMinor, sMajor;
	//OGRSpatialReferenceH iSRS=GDALGetProjectionRef( hDataset );
	/*if (iSRS == NULL) {
		sprintf(msg, "No projection!\n");
		zvmessage(msg,"");
		zabend();
	}*/

	OGRSpatialReferenceH hSRS = NULL;
	const char* pszWkt;
	pszWkt = GDALGetProjectionRef(hDataset);
	if (pszWkt && pszWkt[0])
		hSRS = OSRNewSpatialReference(pszWkt);
	else {
		sprintf(msg, "No projection!\n");
		zvmessage(msg,"");
		zabend();
	}

	double dfULXMap=adfGeoTransform[0];
	double dfULYMap=adfGeoTransform[3];
	double dfXDim=adfGeoTransform[1];
	double dfYDim=adfGeoTransform[5];
	double yulcenter, xulcenter;
	yulcenter = ( dfULYMap / ( -dfYDim * 1. )) - .5;
	xulcenter = ( dfULXMap / ( dfXDim * -1. )) - .5;

	//if (iSRS)
	//	OSRDestroySpatialReference(iSRS);

	//see ogr/ogrsf_frmts/geoconcept/geoconcept_syscoord.c
	//const GCSpheroidInfo* ell= NULL;
	//const GCDatumInfo* datum= NULL;
	//const GCProjectionInfo* gcproj= NULL;

	/*double a, b;
	a=OSRGetSemiMajor(hSRS,NULL);
	b=OSRGetSemiMinor(hSRS,NULL);*/

	//gcproj=OSRGetAttrValue(hSRS, "PROJECTION", 0);
//	printf("proj %s\n", OSRGetAttrValue(hSRS, "PROJECTION", 0));
//	printf("proj %s\n", OSRGetAttrValue(hSRS, "UNIT", 0));
//	printf("proj %s\n", OSRGetAttrValue(hSRS, "PARAMETER", 0));
//	printf("proj %s\n", OSRGetAttrValue(hSRS, "SPHEROID", 1));
//	printf("proj %s\n", OSRGetAttrValue(hSRS, "DATUM", 1));
//	printf("proj %s\n", OSRGetAttrValue(hSRS, "GEOGCS", 0));
//	double  OSRGetProjParm( OGRSpatialReferenceH hSRS,
//	                        const char * pszParmName,
//	                        double dfDefault,
//	                        OGRErr * );
//
//	printf("proj %lf\n", OSRGetProjParm(hSRS,SRS_PP_LATITUDE_OF_ORIGIN,0.0,NULL));
//	printf("proj %lf\n", OSRGetProjParm(hSRS,SRS_PP_CENTRAL_MERIDIAN,0.0,NULL));
//	printf("proj %lf\n", OSRGetProjParm(hSRS,SRS_PP_STANDARD_PARALLEL_1,0.0,NULL));
//	printf("proj %lf\n", OSRGetProjParm(hSRS,SRS_PP_FALSE_EASTING,0.0,NULL));
//	printf("proj %lf\n", OSRGetProjParm(hSRS,SRS_PP_FALSE_NORTHING,0.0,NULL));
//
//	printf("proj %d\n", OSRIsGeocentric(hSRS));
//
//	printf("proj %s\n", pszWkt);

	/*
	sMinor = OSRGetSemiMajor( iSRS, 0 );
	sMajor = OSRGetSemiMinor( iSRS, 0 );
	printf("Node %s\n", OSRGetAttrValue( iSRS, "UNIT", 1));*/

	//if( GDALGetProjectionRef( hDataset ) != NULL )
	//    printf( "Projection is `%s'\n", GDALGetProjectionRef( hDataset ) );



	/*if( GDALGetOverviewCount(hBand) > 0 )
	    printf( "Band has %d overviews.\n", GDALGetOverviewCount(hBand));

	if( GDALGetRasterColorTable( hBand ) != NULL )
		printf( "Band has a color table with %d entries.\n",
				GDALGetColorEntryCount( GDALGetRasterColorTable( hBand ) ) );*/

	// All lines at once
	//for (line=0;line<nYSize;line++)
		//GDALRasterIO( hBand, GF_Read, 0, line, nXSize, 1, (inbuf_float+(line*nXSize)), nXSize, 1, GDT_Float32, 0, 0 );
		//GDALRasterIO( hBand, GF_Read, 0, line, nXSize, 1, (inbuf_half+(line*nXSize)), nXSize, 1, GDT_Int16, 0, 0 );
	//GDALClose(hDataset);

	//status *= zladd (hrstruct->aspect_unit, "PROPERTY", "MDENOISE", &hrstruct->mdenoise, "ERR_ACT", "S", "FORMAT", "STRING", "PROPERTY", "PHOT",NULL);
	//status *= zladd (hrstruct->slope_unit, "PROPERTY", "MDENOISE", &hrstruct->mdenoise, "ERR_ACT", "S", "FORMAT", "STRING", "PROPERTY", "PHOT",NULL);

	status = zvp("out", out_name, &count);
	status = zvunit (&out_unit, out_name,  0, "U_NAME", out_name, NULL);

	/*			DATA STRUCTURES																	*/

	//Earth_prefs		prefs_dtm;

	/*			GET AND CHECK INPUT LABEL ENTRIES												*/
	

	/*			OPEN DTM FILE																	*/

	/*status = mpInit(&(mp_dtm_obj));
	if(status != mpSUCCESS) {
		zvmessage("mpInit failed for dtm !!", "");
		zabend();
	}*/

	/* Read DTM label */
	/*status = hwdtmrl(hrstruct.dtm_unit, &(hrstruct.dtmlabel));
	if(status == -90) {
		sprintf(msg, "HWDTMRL STATUS: %d", status);
		zvmessage(msg,"ERROR");
		zabend();
	}*/
	//dtmlabel.dtm_minimum_dn
	//dtmlabel.dtm_scaling_factor
	//dtmlabel.dtm_offset

	/* Read DTM Map Label */
	//status = dlr_mpLabelRead(hrstruct.mp_dtm_obj, hrstruct.dtm_unit, &prefs_dtm);
	/*status = mpGetValues  ( hrstruct.mp_dtm_obj, mpA_AXIS_RADIUS, &hrstruct.dtm_axes_map[0], NULL);
	    status = mpGetValues  ( hrstruct.mp_dtm_obj, mpB_AXIS_RADIUS, &hrstruct.dtm_axes_map[1], NULL);
	    status = mpGetValues  ( hrstruct.mp_dtm_obj, mpC_AXIS_RADIUS, &hrstruct.dtm_axes_map[2], NULL);
	    hrstruct.dtm_poslongdir_fac = 1.;
	    status = mpGetValues ( hrstruct.mp_dtm_obj, mpPOSITIVE_LONGITUDE_DIRECTION, poslongdir, NULL);
	    if (strcmp(poslongdir, "WEST")==0) hrstruct.dtm_poslongdir_fac = -1.;
	    //hrstruct.cenlat=0.;
	    //hrstruct.cenlong=0.;
	    //status = mpSetValues ( hrstruct.mp_dtm_obj, mpCENTER_LATITUDE, hrstruct.cenlat, NULL);
	    //status = mpSetValues ( hrstruct.mp_dtm_obj, mpCENTER_LONGITUDE, hrstruct.cenlong, NULL);
	    status = mpGetValues ( hrstruct.mp_dtm_obj, mpCENTER_LATITUDE, &(hrstruct.cenlat), NULL);
	    if (status == mpKEYWORD_NOT_SET) {printf("exit, no cen_lat in dtm!\n"); zabend();}
	    status = mpGetValues ( hrstruct.mp_dtm_obj, mpCENTER_LONGITUDE, &(hrstruct.cenlong), NULL);
	    if (status == mpKEYWORD_NOT_SET) {printf("exit, no cen_long in dtm!\n"); zabend();}
	    status = dlr_earth_map_get_prefs (hrstruct.mp_dtm_obj, &prefs_dtm);
	    if (status != 1) {
	    	printf("Error in dlr_earth_map_get_prefs 1!");
	    	zabend();
		}*/

	//status = zlget (hrstruct.in_unit, "PROPERTY", "MAP_SCALE", &temp_float, "PROPERTY", "MAP", "FORMAT", "REAL", NULL);
	/*if (status != 1) {
		zvmessage(" MAP item MAP_SCALE missing ! Please set MP_SCALE !!","");
		zabend();
		} else hrstruct.scale_resolution=temp_float;*/
	//status = zvopen(hrstruct.in_unit, "OP", "READ", "open_act", "AUS", "open_mes", "Error opening input file", "COND", "BINARY", "U_FORMAT", "REAL", NULL);
	//status = zvopen (hrstruct.slope_unit, "U_NL", hrstruct.nof_dtm_l, "U_NS", hrstruct.nof_dtm_s, "OP", "WRITE", "OPEN_ACT", "SA", "O_FORMAT", "REAL", "U_FORMAT", "REAL", NULL);
	//status = zvopen (hrstruct.aspect_unit, "U_NL", hrstruct.nof_dtm_l, "U_NS", hrstruct.nof_dtm_s, "OP", "WRITE", "OPEN_ACT", "SA", "O_FORMAT", "REAL", "U_FORMAT", "REAL", NULL);
	//status = zvopen (hrstruct.out_unit, "U_NL", hrstruct.nof_inp_l, "U_NS", hrstruct.nof_inp_s, "OP", "WRITE", "OPEN_ACT", "SA", "O_FORMAT", "REAL", "U_FORMAT", "REAL", NULL);
	/*outbuf   = (float *) malloc (nXSize*sizeof(float));
	if (outbuf == (float *)NULL) {
		sprintf(msg, "Error during allocation of output buffer!!");
		zvmessage(msg,"");
		zabend();
	}*/
	status = zvopen (out_unit, "U_NL", nYSize, "U_NS", nXSize, "U_NB", 1, "OP", "WRITE", "OPEN_ACT", "SA", "O_FORMAT", "HALF", "U_FORMAT", "HALF", NULL);

	MP mp_obj;
	status = mpInit(&mp_obj);
	status = mpSetValues(mp_obj, mpCOORDINATE_SYSTEM_NAME, "PLANETOCENTRIC", NULL);
	status = mpSetValues(mp_obj, mpA_AXIS_RADIUS, OSRGetSemiMajor( hSRS, 0 )/1000., NULL);
	status = mpSetValues(mp_obj, mpB_AXIS_RADIUS, OSRGetSemiMinor( hSRS, 0 )/1000., NULL);
	status = mpSetValues(mp_obj, mpC_AXIS_RADIUS, OSRGetSemiMinor( hSRS, 0 )/1000., NULL);
	status = mpSetValues(mp_obj, mpCENTER_LATITUDE, OSRGetProjParm(hSRS,SRS_PP_LATITUDE_OF_ORIGIN,0.0,NULL), NULL);
	status = mpSetValues(mp_obj, mpCENTER_LONGITUDE, OSRGetProjParm(hSRS,SRS_PP_CENTRAL_MERIDIAN,0.0,NULL), NULL);
	//if (!strcmp(OSRGetAttrValue(hSRS, "PROJECTION", 0),"Equirectangular"))
	//status = mpSetValues(mp_obj, mpMAP_PROJECTION_TYPE, mpEQUIDISTANT, NULL);
	status = mpSetValues(mp_obj, mpMAP_PROJECTION_TYPE, mpSIMPLE_CYLINDRICAL, NULL);
	//if (!strcmp(OSRGetAttrValue(hSRS, "PROJECTION", 0),"Sinusoidal"))
	//	status = mpSetValues(mp_obj, mpMAP_PROJECTION_TYPE, mpSINUSOIDAL, NULL);
	status = mpSetValues(mp_obj, mpPOSITIVE_LONGITUDE_DIRECTION, "EAST", NULL);
	//status = mpSetValues ( mp_obj, mpMAP_SCALE, dfXDim/1000., NULL);
	status = mpSetValues ( mp_obj, mpMAP_SCALE, 200.0, NULL);
	status = mpSetValues ( mp_obj, mpMAP_RESOLUTION, 296.3735, NULL);
	status = mpSetValues ( mp_obj, mpSAMPLE_PROJECTION_OFFSET, xulcenter, NULL);
	status = mpSetValues ( mp_obj, mpLINE_PROJECTION_OFFSET, yulcenter, NULL);
	status = mpSetValues ( mp_obj, mpBODY_LONG_AXIS, 0.0, NULL);
	status = mpSetValues ( mp_obj, mpFIRST_STANDARD_PARALLEL, OSRGetProjParm(hSRS,SRS_PP_STANDARD_PARALLEL_1,0.0,NULL), NULL);
	status = mpSetValues ( mp_obj, mpSECOND_STANDARD_PARALLEL, OSRGetProjParm(hSRS,SRS_PP_STANDARD_PARALLEL_2,0.0,NULL), NULL);
	status = mpSetValues ( mp_obj, mpSPHERICAL_AZIMUTH, 0.0, NULL);
	status = mpSetValues ( mp_obj, mpCARTESIAN_AZIMUTH, 0.0, NULL);

	char *ptr;
	char geogcs[255];
	strcpy(geogcs,OSRGetAttrValue(hSRS, "GEOGCS", 0));
	if (!strncmp(geogcs,"GCS_",3)) {
		ptr = strtok(geogcs, "_");
		ptr = strtok(NULL, "_");
		status = mpSetValues ( mp_obj, mpTARGET_NAME, ptr);
	}

	status = mpLabelWrite( mp_obj, out_unit, "PROPERTY");
	status = mpLabelWrite( mp_obj, out_unit, "HISTORY");
	if (status != mpSUCCESS) {
		sprintf(msg, "Error in mpLabelWrite !! %d", status);
		zvmessage(msg,"");
		zabend();
	}

	status = zvp ( "DTM", file_is_dtm, &count);
	if (!strcmp(file_is_dtm,"YES")) {
		dtm			dtmlabel;
		int bGotMin, bGotMax;
		double adfMinMax[2];
		/*adfMinMax[0] = GDALGetRasterMinimum( hBand, &bGotMin );
		adfMinMax[1] = GDALGetRasterMaximum( hBand, &bGotMax );
		if( ! (bGotMin && bGotMax) )
		    GDALComputeRasterMinMax( hBand, TRUE, adfMinMax );
		//printf( "Min=%.3fd, Max=%.3f\n", adfMinMax[0], adfMinMax[1] );
		dtmlabel.dtm_minimum_dn=adfMinMax[0];
		dtmlabel.dtm_maximum_dn=adfMinMax[1];*/
		dtmlabel.dtm_minimum_dn=-8528;
		dtmlabel.dtm_maximum_dn=21226;
		dtmlabel.dtm_missing_dn=-32768;
		dtmlabel.dtm_offset=0.0;
		dtmlabel.dtm_scaling_factor=1.0;
		dtmlabel.dtm_a_axis_radius=OSRGetSemiMajor( hSRS, 0 )/1000.;
		dtmlabel.dtm_b_axis_radius=OSRGetSemiMinor( hSRS, 0 )/1000.;
		dtmlabel.dtm_c_axis_radius=OSRGetSemiMinor( hSRS, 0 )/1000.;
		dtmlabel.dtm_body_long_axis=0.0;
		strcpy(dtmlabel.dtm_positive_longitude_direction,"EAST");
		strcpy(dtmlabel.dtm_height_direction,"RADIAL");
		strcpy(dtmlabel.dtm_desc,"http://bit.ly/HRSC_MOLA_Blend_v0");
		status = hwdtmwl(out_unit, &dtmlabel);
	}

	OSRDestroySpatialReference(hSRS);

	//write line by line
	for (line=0;line<nYSize;line++) {
		//GDALRasterIO( hBand, GF_Read, 0, line, nXSize, 1, in_line_buf_half, nXSize, 1, GDT_Float32, 0, 0 );
		GDALRasterIO( hBand, GF_Read, 0, line, nXSize, 1, in_line_buf_half, nXSize, 1, GDT_Int16, 0, 0 );
		status *= zvwrit (out_unit, in_line_buf_half, "LINE", line+1, NULL);
	}
	GDALClose(hDataset);
	/*status = zladd (hrstruct.out_unit, "PROPERTY", "MINIMUM", &hrstruct.out_min, "ERR_ACT", "S", "FORMAT", "REAL", "PROPERTY", "STATISTICS", NULL);
	status = zladd (hrstruct.out_unit, "PROPERTY", "MAXIMUM", &hrstruct.out_max, "ERR_ACT", "S", "FORMAT", "REAL", "PROPERTY", "STATISTICS", NULL);
	status = zladd (hrstruct.out_unit, "PROPERTY", "MEAN", &hrstruct.out_mean, "ERR_ACT", "S", "FORMAT", "REAL", "PROPERTY", "STATISTICS", NULL);
	status = zladd (hrstruct.out_unit, "PROPERTY", "STANDARD_DEVIATION", &hrstruct.out_stddev, "ERR_ACT", "S", "FORMAT", "REAL", "PROPERTY", "STATISTICS", NULL);

	status = zladd (hrstruct.out_unit, "HISTORY", "INPUT", &hrstruct.in_name, "ERR_ACT", "S", "FORMAT", "STRING", NULL );
	status = zladd (hrstruct.out_unit, "HISTORY", "DTM_NAME", &hrstruct.dtm_name, "ERR_ACT", "S", "FORMAT", "STRING", NULL );
	status = zladd (hrstruct.out_unit, "HISTORY", "PHOFUNC", &phostruct.phofunc, "ERR_ACT", "S", "FORMAT", "STRING", NULL );
	status = zladd (hrstruct.out_unit, "HISTORY", "METHOD", &phostruct.method, "ERR_ACT", "S", "FORMAT", "STRING", NULL );*/

	status = zvclose(out_unit, NULL);

	free(inbuf_half);
	free(inbuf_float);

	sprintf(msg, "VICAR task tif2vic completed ");
    zvmessage(msg,"MAINPR");

}


