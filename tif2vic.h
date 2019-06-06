//#define NULL ((void *) 0)

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <alloca.h>
#include <sys/stat.h>

#include	"SpiceUsr.h"
#include	"zvproto.h"
#include	"mp_routines.h"
#include	"hrpref.h"
#include	"dlrmapsub.h"
#include	"dtm.h"
#include	"extori.h"
#include	"hrgetversion.h"
#include	"hrgcl.h"
#include	"hrgetstdscale.h"
#include	"hwldker.h"
#include	"gsl/gsl_statistics.h"
#include	"gsl/gsl_sort.h"
#include	"gsl/gsl_fit.h"

#define MAX_STRING_SIZE 120
#define MSG_STRING_SIZE 80
#define USEEXTORI 1
//#define MAX_PIXEL_PER_LINE TOTAL_ACTIVE_PIXEL
#define BUFLEN 50000
#define USESPICE 0
#define DOELL 3
#define DOMODEL 2
#define	PI  3.14159265358979323
#define MAXLINESIZE  1000
#define MAX_PIXEL_PER_LINE TOTAL_ACTIVE_PIXEL
typedef unsigned char myBYTE;
#define MAX_STAT_SIZE 100000
#define MINFIT 1
#define NUMFIT 18

typedef struct {
	char   phofunc[MAX_STRING_SIZE];
	char   method[MAX_STRING_SIZE];
	double min_kparam;
	double min_albedo;
	double ccor_cval;
	double hapke_w;
	double hapke_b;
	double hapke_c;
	double hapke_h;
	double hapke_b0;
	double hapke_theta;
} phot_type;


typedef struct	{
//Input image
	int			in_unit;
	char		in_name[MAX_STRING_SIZE];
	int			in_nbb;
	int			out_nbb;
	int			in_lblsize;
	char		in_format[MAX_STRING_SIZE];
	char		in_type[MAX_STRING_SIZE];
	int			in_level;
	char		in_target[MAX_STRING_SIZE];
	char		in_bltype[MAX_STRING_SIZE];
	double		in_radsf; //radiance_scaling_factor
	double		in_rados; //radiance_offset
	double		in_refsf; //reflectance_scaling_factor
	double		in_refos;
	int			sl_inp;
	int			nl_inp;
	int			nl;
	int			ns;
	int			sl;
	int			ss;
	double		ignore;
	char		target_name[MAX_STRING_SIZE];
	int			target_id;
	int			nof_inp_l;
	int			nof_inp_s;
	double		axes[3];
	int			mp_radius_set;
	char		det_id[MAX_STRING_SIZE];
	char		spacecraft_name[MAX_STRING_SIZE];
	int			first_used_l;
	int			last_used_l;
	int			shadow;
	float		in_mean;
	float		in_min;
	float		in_max;
	float		in_stddev;
//Output image
	char		out_name[MAX_STRING_SIZE];
	int			out_unit;
//	char		out_format[MAX_STRING_SIZE];
	int			illu_unit[9];
	char		illu_format[MAX_STRING_SIZE];
	double		outmax;
	int			slope_unit;
	int			aspect_unit;
	float		out_min;
	float		out_max;
	float		out_mean;
	float		out_stddev;
//DTM image
	int			dtm_unit;
	char		dtm_name[MAX_STRING_SIZE];
	char		mdenoise[MAX_STRING_SIZE];
	//long		ram_dtm;
	int			nof_dtm_l;
	int			nof_dtm_s;
	dtm			dtmlabel;
	MP			mp_dtm_obj;
	int			ell_unit[3];
//EXTORI file
	char		extorifile[MAX_STRING_SIZE];
	double		*time;
	double		focal;
	double		*xcal;
	double		*ycal;
	double		*xyz0;
	double		*xyz1;
	double		*xyz2;
	double		*phi;
	double		*omega;
	double		*kappa;
//Parameters
	char		phofunc[MAX_STRING_SIZE];
	char		method[MAX_STRING_SIZE];
	float		kparam;
	//float		aem;
	float		tval;
	char		auxout[MAX_STRING_SIZE];
	char		auxcorr[MAX_STRING_SIZE];
	char		reflout[MAX_STRING_SIZE];
	char		radiout[MAX_STRING_SIZE];
	char		ipol[MAX_STRING_SIZE];
	char		latlon[MAX_STRING_SIZE];
	double		ll[2];
	char		instrument_id[MAX_STRING_SIZE];
	char		detector_id[MAX_STRING_SIZE];
	char		target[MAX_STRING_SIZE];
	char		fixref[MAX_STRING_SIZE];
	char		geocal_dir[MAX_STRING_SIZE];
	//int			geocal_version;
	double		ignore_min;
	double		ignore_max;
	char		gcal_filename[MAX_STRING_SIZE];
	char		geocal_version[80];
	char		ins_id[6];
	char		ins_name[MAX_STRING_SIZE];
	int			parts;
	char		usedtm[MAX_STRING_SIZE];
//IMAGE
	//int			first_inp_l_part;
	//int			nof_inp_l_part;
	int			anchdist;
	int			match;
	//int			nof_out_l_part;
	//int			nof_out_s_part;
	int			nof_out_l;
	int			nof_out_s;
	int			oformat;
	int			nbb;
	int			first_io;
	//int			first_out_l_part;
	//int			first_out_s_part;
	long   		*index_x;
	long   		*index_y;
	FILE		*new_match_x_unit;
	FILE		*new_match_y_unit;
	float		no_info_val;
	int			interpol_type;
	int			macro;
	int			fp;
	int			fap;
	int			fillp;
	int			non_active_pixel_start;
	double		match_prec;
	int			quad[4];
//Map
	double		long_axis;
	 int	fittofile;
	 int	border;
	char		tmp[MAX_STRING_SIZE];
	float		maxmem;
	//int			geom;
	double		min_h_in_dtm;
	double		max_h_in_dtm;
	double		dtm_axes[3];
	double		dtm_axes_map[3];
	double		dtm_poslongdir_fac;
	double		poslongdir_fac;
	//int			*dtm_tab_in;
	//double		height;
	int			found;
	int			found_in_dtm;
	double		mp_axes[3];
	double		min_lati;
	double		min_longi;
	double		max_lati;
	double		max_longi;
	char		mptype[mpMAX_KEYWD_LENGTH+1];
	double		cenlong;
	double		cenlat;
	int			critical_projection;
	double		scale_resolution;

} hrstruct_type;

int getparameters (hrstruct_type *hrstruct, phot_type *phostruct);
int	getlabel (hrstruct_type *hrstruct);
void check_size(hrstruct_type *hrstruct);
int dtm2aspectslope (hrstruct_type *hrstruct);
int instats (hrstruct_type *hrstruct);
int fitpatches (hrstruct_type *hrstruct, Earth_prefs prefs_dtm, phot_type *phostruct);
int calcpho (hrstruct_type *hrstruct, Earth_prefs prefs_dtm, phot_type *phostruct);
void StartStatus(long maxval);
void UpdateStatus(long current);
extern float  hwpixnum(float numb, int macro, int fip, int mode);
int hwgetpar( MP *mp, int target_id );
int find_hist_key(int unit, char *key, int lastflag, char *task, int *instance);
int hwldker_error (int number, char message[HWLDKER_ERROR_LENGTH] );
void printrotmat (SpiceDouble rotmat[3][3]);
int outstats (hrstruct_type *hrstruct, double *buffer);
double minnaert ( phot_type *phostruct, double mu, double munot, double *value);
int normal2topocf(SpiceDouble normal[3], SpiceDouble topocf[3][3]);
int rotatedNormalfromAspectSlope (SpiceDouble aspect, SpiceDouble slope, SpiceDouble rotatedvector[3]);
double hapke2012(phot_type *phostruct, double inc, double emi, double pha);
double hapkesmooth(phot_type *phostruct, double inc, double emi, double pha);
double hapke_isis(double phase, double incidence, double emission, phot_type *phostruct);
double Hfunc(double u, double gamma);
double RADF(phot_type *phostruct, double inc, double emi, double pha, double *radf);
