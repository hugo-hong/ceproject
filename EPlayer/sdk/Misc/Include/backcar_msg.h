#ifndef __BACKCAR__MSG__H_
#define __BACKCAR__MSG__H_


#define WM_FSC_BACKCAR          (WM_USER + 0X7000)
#define FSC_BACKCAR_STATUS_FIN         (0X2)
#define FSC_BACKCAR_STATUS_START    (0X4)



#define WM_VIDEORENDER_MSG                    (WM_USER + 0X7002)
#define VIDEORENDER_MSG_RELEASE_VDP_FIN         (0X2)



#define WM_FSC_BACKCAR_ACK_MSG                (WM_USER + 0X7004)
#define FSC_BACKCAR_ACK_RELEASE_RES_FIN         (0X2)

#ifdef SAMPLE_CODE

  PostMessage(HWND_BROADCAST,WM_FSC_BACKCAR_ACK_MSG,FSC_BACKCAR_ACK_RELEASE_RES_FIN,0);
#endif

#endif
