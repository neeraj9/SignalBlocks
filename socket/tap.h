// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef _c_tap_h
#define _c_tap_h

#ifdef __cplusplus
extern "C" {
#endif

  int tap(const char *device, int promisc_mode);
  int get_ifc_info(char *ifc_name, unsigned int *ip, char *mac);

#ifdef __cplusplus
}
#endif

#endif // _c_tap_h
