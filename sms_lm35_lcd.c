#include <16f877a.h>
#device  PIC16F877A*=16 ADC=10
#include <string.h>
#include <lcd_lib_4bit.c>
#fuses nowdt,hs,noput,noprotect,nodebug,nobrownout,nolvp,nocpd,nowrt
#use delay(clock=12000000)
#use fast_io(b)
#use fast_io(e)
#byte porta=0x05
#byte portb=0x06
#byte portc=0x07
#byte portd=0x08
#byte porte=0x09
#bit b1=portb.0
#bit b2=portb.1
#bit b3=portb.2
#bit b4=portb.3
#bit bt=porte.0   // dau thu tin hieu bao trom
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)
char chuoi[90],noidung_sms[20],tu_khoa[20];
int8 dem,i,j,k,new_sms,z,vitri,x,y,adc,f;
/*********** Khai bao ngat nhan noi tiep ***********/
#INT_RDA
void ngat()
  { 
  chuoi[dem]=getc();   
  if(chuoi[dem]=='M')
  {
  z++;
  if(z==2)new_sms=1;
  }
  dem++;  
  }
void xuly();
void xuly_sms();
void gsm_config();
void xoa();
void trang_thai();
void nhiet_do();
void main()
  {
   set_tris_a(0b000001);
   set_tris_b(0b00000000);
   set_tris_d(0b00000000); 
   set_tris_e(0b111);   
   portb=0;
   portd=0;
   enable_interrupts(global);//Cho phep ngat toan cuc
   enable_interrupts(int_rda);//Cho phep ngat nhan noi tiep
   setup_adc(ADC_clock_internal); // thiet lap tan so lay mau ADC
   setup_adc_ports(AN0);   // ngo vao A0
   set_adc_channel(0);     //  chan doc gia tri adc bang read_adc()
   portd=1;delay_ms(1000);portd=0;delay_ms(1000);  //1 led sang
   LCD_init();   //khoi tao LCD
   Printf(LCD_putchar," DKTB QUA SMS ");
   LCD_putcmd(0xC0);Printf(LCD_putchar,"Dang cau hinh ");delay_ms(1000);
   gsm_config();//goi chuong trinh con cau hinh Module GSM    
   LCD_putcmd(0xC0);Printf(LCD_putchar,"Cau hinh xong ");delay_ms(2000);
   new_sms=0;
   z=0;   
   while(1)
   { 
     signed int16 x;
     signed int16 adc;
     int16 y=70; // nhiet do bao dong
     x=read_ADC();
     adc=x/2; // do C
     f=adc*1.8+32;  // do F
     LCD_putcmd(0x01);// xoa man hinh chi thi
     LCD_putcmd(0x80);Printf(LCD_putchar," Nhiet do phong ");
     LCD_putcmd(0xC0);Printf(LCD_putchar,"%u",adc);LCD_putcmd(0xC0+4);Printf(LCD_putchar,"C");
     LCD_putcmd(0xC0+8);Printf(LCD_putchar,"%u",f);LCD_putcmd(0xC0+12);Printf(LCD_putchar,"F");
     delay_ms(500);
     if(new_sms==1)
      {
      LCD_putcmd(0x01);// xoa man hinh chi thi
      LCD_putcmd(0x80);   Printf(LCD_putchar,"Co tin nhan moi ");
      LCD_putcmd(0xC0);   Printf(LCD_putchar,"Dang xu ly");
      xoa();
      puts("at+cmgr=1");
      delay_ms(500);
      portd=15;delay_ms(500);portd=0;delay_ms(500);  //4 led  sang 
      xuly_sms();
      LCD_putcmd(0x01);// xoa man hinh chi thi
      xuly();
      trang_thai();
      nhiet_do();
      delay_ms(200);      
      portd=31;delay_ms(500);portd=0;delay_ms(500);  //5 led  sang 
      puts("at+cmgd=1");
      delay_ms(500); 
      xoa();
      delay_ms(500);
      portd=63;delay_ms(500);portd=0;delay_ms(500);  //6 led  sang         
      }
      if(adc>=y)  // kiem tra nhiet do cao
      {
      LCD_putcmd(0x01);// xoa man hinh chi thi
      LCD_putcmd(0x80);Printf(LCD_putchar," Nhiet do cao ! ");
      LCD_putcmd(0xC0);Printf(LCD_putchar,"Dang goi dien ");
      printf("ATD01674577163;"); 
      putc(13);
      delay_ms(20000);
      puts("ATH");  
      delay_ms(1000); 
      printf("AT+CMGS=\"01674577163\"\r\n"); 
      delay_ms(500);
      printf("Nhiet do luc nay la:\r\n %u",adc);   // gui nhiet do ve may
      delay_ms(500);
      printf("do C");
      delay_ms(500);
      putc(26);  // ctrl +Z 
      delay_ms(500);
      }
      if(bt==0)  // kiem tra co trom khong
      {
      LCD_putcmd(0x01);// xoa man hinh chi thi
      LCD_putcmd(0x80);LCD_putcmd(0x06);Printf(LCD_putchar," Co nguoi vao   ! ");
      LCD_putcmd(0xC0);LCD_putcmd(0x06);Printf(LCD_putchar,"Dang goi dien  ");
      printf("ATD01674577163;"); 
      putc(13);
      delay_ms(20000);
      puts("ATH");  
      delay_ms(1000);
      printf("AT+CMGS=\"01674577163\"\r\n"); 
      delay_ms(500);
      printf("Co nguoi dot nhap");   // 
      delay_ms(500);      
      putc(26);  // ctrl +Z 
      delay_ms(500);     
      }
        }}    
void gsm_config()   
  {      
   delay_ms(2000);
   printf("at\r\n");
   delay_ms(500);
   printf("at\r\n");
   delay_ms(500);
   printf("at\r\n");
   delay_ms(500);
   printf("at\r\n");
   delay_ms(500);
   printf("ate0\r\n");
   delay_ms(500);
   printf("at+cmgf=1\r\n");
   delay_ms(500);
   printf("at+cmgd=1\r\n");
   delay_ms(500);
   xoa();
   delay_ms(500);
   portd=3;delay_ms(1000);portd=0;delay_ms(1000); //2 led  sang //cau hinh xong
   }
//+++++++++++++++ xoa +++++++++++++++++//
void xoa()
  {
  new_sms=0;
  z=0; 
  dem=0;  
  y=0;
  vitri=0;
 for(i=0;i<=90;i++) //Xoa chuoi[]
   {chuoi[i]=0;}
 for(i=0;i<=20;i++) //Xoa noidung_sms[]
   {noidung_sms[i]=0;}
   }
//+++++++++++++++++++++ xu ly tin nhan  ++++++++++++++//
void xuly_sms()
{
   y=0;
   k=0;
   for(i=0;i<=strlen(chuoi);i++)          
       {
          if(chuoi[i]=='\n')               
           {
             k++;                                  
             if(k==2)vitri=i+1 ;              
           }
       }
   for(x=vitri;x<strlen(chuoi)-8;x++)
      {noidung_sms[y]=chuoi[x];
       y++ ;  // lay noi dung
      }  
}
//+++++++++++++++ XU LY va DIEU KHIEN +++++++++++++++++++//
void xuly()
{  
   strcpy(tu_khoa,"On1");
   if(strcmp(tu_khoa,noidung_sms)==0)
      {
      b1=1;
      LCD_putcmd(0x80);   Printf(LCD_putchar,"Den 1 da mo ");
      LCD_putcmd(0xC0);   Printf(LCD_putchar,"Gui sms p.hoi");
      printf("AT+CMGS=\"01674577163\"\r\n"); 
      delay_ms(500);
      printf("den 1 da mo");
      delay_ms(500);
      putc(26);  // ctrl +Z 
      delay_ms(500);
     }         
   strcpy(tu_khoa,"Off1");
   if(strcmp(tu_khoa,noidung_sms)==0)
      {
      b1=0;
      LCD_putcmd(0x80);   Printf(LCD_putchar,"Den 1 da tat ");
      LCD_putcmd(0xC0);   Printf(LCD_putchar,"Gui sms p.hoi");
      printf("AT+CMGS=\"01674577163\"\r\n"); 
      delay_ms(500);
      printf("den 1 da tat");
      delay_ms(500);
      putc(26);  // ctrl +Z 
      delay_ms(500);
      }
   strcpy(tu_khoa,"On2");
   if(strcmp(tu_khoa,noidung_sms)==0)
      {
      b2=1;
      LCD_putcmd(0x80);   Printf(LCD_putchar,"Den 2 da mo ");
      LCD_putcmd(0xC0);   Printf(LCD_putchar,"Gui sms p.hoi");
      printf("AT+CMGS=\"01674577163\"\r\n"); 
      delay_ms(500);
      printf("den 2 da mo");
      delay_ms(500);
      putc(26);  // ctrl +Z 
      delay_ms(500);
      }
   strcpy(tu_khoa,"Off2");
   if(strcmp(tu_khoa,noidung_sms)==0)
      {
      b2=0;
      LCD_putcmd(0x80);   Printf(LCD_putchar,"Den 2 da tat ");
      LCD_putcmd(0xC0);   Printf(LCD_putchar,"Gui sms p.hoi");
      printf("AT+CMGS=\"01674577163\"\r\n"); 
      delay_ms(500);
      printf("den 2 da tat");
      delay_ms(500);
      putc(26);  // ctrl +Z 
      delay_ms(500);
      }
   strcpy(tu_khoa,"On3");
   if(strcmp(tu_khoa,noidung_sms)==0)
      {
      b3=1;
      LCD_putcmd(0x80);   Printf(LCD_putchar,"Den 3 da mo ");
      LCD_putcmd(0xC0);   Printf(LCD_putchar,"Gui sms p.hoi");
      printf("AT+CMGS=\"01674577163\"\r\n"); 
      delay_ms(500);
      printf("den 3 da mo");
      delay_ms(500);
      putc(26);  // ctrl +Z 
      delay_ms(500);
      }
   strcpy(tu_khoa,"Off3");
   if(strcmp(tu_khoa,noidung_sms)==0)
      {
      b3=0;
      LCD_putcmd(0x80);   Printf(LCD_putchar,"Den 3 da tat ");
      LCD_putcmd(0xC0);   Printf(LCD_putchar,"Gui sms p.hoi");
      printf("AT+CMGS=\"01674577163\"\r\n"); 
      delay_ms(500);
      printf("den 3 da tat");
      delay_ms(500);
      putc(26);  // ctrl +Z 
      delay_ms(500);
      }
   strcpy(tu_khoa,"On4");
   if(strcmp(tu_khoa,noidung_sms)==0)
      {
      b4=1;
      LCD_putcmd(0x80);   Printf(LCD_putchar,"Den 4 da mo ");
      LCD_putcmd(0xC0);   Printf(LCD_putchar,"Gui sms p.hoi");
      printf("AT+CMGS=\"01674577163\"\r\n"); 
      delay_ms(500);
      printf("den 4 da mo");
      delay_ms(500);
      putc(26);  // ctrl +Z 
      delay_ms(500);
      }
   strcpy(tu_khoa,"Off4");
   if(strcmp(tu_khoa,noidung_sms)==0)
      {
      b4=0;
      LCD_putcmd(0x80);   Printf(LCD_putchar,"Den 4 da tat ");
      LCD_putcmd(0xC0);   Printf(LCD_putchar,"Gui sms p.hoi");
      printf("AT+CMGS=\"01674577163\"\r\n"); 
      delay_ms(500);
      printf("den 4 da tat");
      delay_ms(500);
      putc(26);  // ctrl +Z 
      delay_ms(500);
      }
   strcpy(tu_khoa,"Onall");
   if(strcmp(tu_khoa,noidung_sms)==0)
      {
      portb=255;
      LCD_putcmd(0x80);   Printf(LCD_putchar,"Tat ca den mo");
      LCD_putcmd(0xC0);   Printf(LCD_putchar,"Gui sms p.hoi");
      printf("AT+CMGS=\"01674577163\"\r\n"); 
      delay_ms(500);
      printf("tat ca den da mo");
      delay_ms(500);
      putc(26);  // ctrl +Z 
      delay_ms(500);
      }
   strcpy(tu_khoa,"Offall");
   if(strcmp(tu_khoa,noidung_sms)==0)
      {
      portb=0;
      LCD_putcmd(0x80);   Printf(LCD_putchar,"Tat ca den tat ");
      LCD_putcmd(0xC0);   Printf(LCD_putchar,"Gui sms p.hoi");
      printf("AT+CMGS=\"01674577163\"\r\n"); 
      delay_ms(500);
      printf("tat ca den da tat");
      delay_ms(500);
      putc(26);  // ctrl +Z 
      delay_ms(500);
      }
}
void trang_thai()
{   
   strcpy(tu_khoa,"Status");
   if(strcmp(tu_khoa,noidung_sms)==0)
      {
          LCD_putcmd(0x80);   Printf(LCD_putchar,"Dang kiem tra");
          LCD_putcmd(0xC0);   Printf(LCD_putchar,"Gui sms p.hoi");
          printf("AT+CMGS=\"01674577163\"\r\n");
          delay_ms(500);  
          if(b1==1)
          {
          printf("den 1 dang mo\r\n");
          delay_ms(500);
          }
          if(b1==0)
          {
          printf("den 1 dang tat\r\n");
          delay_ms(500);
          }
          if(b2==1)
          {
          printf("den 2 dang mo\r\n");
          delay_ms(500);
          }
          if(b2==0)
          {
          printf("den 2 dang tat\r\n");
          delay_ms(500);
          }
          if(b3==1)
          {
          printf("den 3 dang mo\r\n");
          delay_ms(500);
          }
          if(b3==0)
          {
           printf("den 3 dang tat\r\n");
           delay_ms(500);
          }
          if(b4==1)
          {
          printf("den 4 dang mo\r\n");
          delay_ms(500);          
          }
          if(b4==0)
          {
          printf("den 4 dang tat\r\n");
          delay_ms(500);         
          }
       putc(26);  // ctrl +Z 
       delay_ms(500);   
    }
}
void nhiet_do()
{
   strcpy(tu_khoa,"nhietdo");
   if(strcmp(tu_khoa,noidung_sms)==0)
      {
      LCD_putcmd(0x80);   Printf(LCD_putchar,"Kiem tra nhiet ");
      LCD_putcmd(0xC0);   Printf(LCD_putchar,"do ve dt      ");
      printf("Nhiet do luc nay la:\r\n %u",adc);   // gui nhiet do ve may
      delay_ms(500);
      printf("do C");
      delay_ms(500);
      putc(26);  // ctrl +Z 
      delay_ms(500);
     }    
}
