;InnoSetupVersion=5.5.0

[Setup]
AppName=��������
AppVerName=�桤ҡǮ��
DefaultDirName={reg:HKCU\Software\WHQPGame\Plaza,Path}
OutputBaseFilename=zyqs
Compression=zip
PrivilegesRequired=none
WizardImageFile=embedded\WizardImage.bmp
WizardSmallImageFile=embedded\WizardSmallImage.bmp

[Files]
Source: "{app}\lkbass.dll"; DestDir: "{app}"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\guichan.dll"; DestDir: "{app}"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\hgeencrypt.dll"; DestDir: "{app}"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\magic.dll"; DestDir: "{app}"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs.exe"; DestDir: "{app}"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\Chair.png"; DestDir: "{app}\zyqs"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\content.fish"; DestDir: "{app}\zyqs"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\GameLevel.INI"; DestDir: "{app}\zyqs"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\Locker.bmp"; DestDir: "{app}\zyqs"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\TABLE.png"; DestDir: "{app}\zyqs"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\TableResource.INI"; DestDir: "{app}\zyqs"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\UserReady.bmp"; DestDir: "{app}\zyqs"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\coin1.fish"; DestDir: "{app}\zyqs\images"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\coin2.fish"; DestDir: "{app}\zyqs\images"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\credit_num.fish"; DestDir: "{app}\zyqs\images"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cursor.fish"; DestDir: "{app}\zyqs\images"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\help_tip.fish"; DestDir: "{app}\zyqs\images"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\jetton.fish"; DestDir: "{app}\zyqs\images"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\jetton_bgc1.fish"; DestDir: "{app}\zyqs\images"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\jetton_bgc2.fish"; DestDir: "{app}\zyqs\images"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\jetton_num.fish"; DestDir: "{app}\zyqs\images"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\loading_bar.fish"; DestDir: "{app}\zyqs\images"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\logo.fish"; DestDir: "{app}\zyqs\images"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\message_bg.fish"; DestDir: "{app}\zyqs\images"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\score_box.fish"; DestDir: "{app}\zyqs\images"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\score_num.fish"; DestDir: "{app}\zyqs\images"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\score_num_small.fish"; DestDir: "{app}\zyqs\images"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\selfinfo.fish"; DestDir: "{app}\zyqs\images"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\user_score.fish"; DestDir: "{app}\zyqs\images"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet1_ion.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet1_norm1.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet1_norm10.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet1_norm2.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet1_norm3.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet1_norm4.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet1_norm5.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet1_norm6.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet1_norm7.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet1_norm8.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet1_norm9.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet2_ion.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet2_norm1.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet2_norm10.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet2_norm2.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet2_norm3.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet2_norm4.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet2_norm5.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet2_norm6.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet2_norm7.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet2_norm8.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet2_norm9.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet3_ion.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet3_norm1.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet3_norm10.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet3_norm2.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet3_norm3.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet3_norm4.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet3_norm5.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet3_norm6.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet3_norm7.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet3_norm8.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet3_norm9.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet4_ion.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet4_norm1.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet4_norm10.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet4_norm2.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet4_norm3.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet4_norm4.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet4_norm5.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet4_norm6.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet4_norm7.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet4_norm8.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\bullet\bullet4_norm9.fish"; DestDir: "{app}\zyqs\images\bullet"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\board.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\cannon_num.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\card_ion.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\deco1.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\deco10.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\deco2.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\deco3.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\deco4.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\deco5.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\deco6.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\deco7.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\deco8.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\deco9.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\disc.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\gun1.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\gun1_1.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\gun1_2.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\gun1_3.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\gun2.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\gun2_1.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\gun2_2.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\gun2_3.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\GunFire.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\cannon\seat.fish"; DestDir: "{app}\zyqs\images\cannon"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\dish.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish1.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish10.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish10_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish11.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish11_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish12.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish12_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish13.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish13_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish14.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish14_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish15.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish15_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish16.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish16_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish17.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish17_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish18.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish18_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish19.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish19_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish1_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish2.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish20.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish20_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish21.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish21_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish22.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish22_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish23.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish23_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish24.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish24_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish2_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish3.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish3_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish4.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish4_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish5.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish5_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish6.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish6_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish7.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish7_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish8.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish8_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish9.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\fish9_d.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Fish\halo.fish"; DestDir: "{app}\zyqs\images\Fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\gui\BT_CANCLE.fish"; DestDir: "{app}\zyqs\images\gui"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\gui\BT_OK.fish"; DestDir: "{app}\zyqs\images\gui"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\gui\setDlg.fish"; DestDir: "{app}\zyqs\images\gui"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\gui\setDlgCheck1.fish"; DestDir: "{app}\zyqs\images\gui"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\gui\SetDlgPitch.fish"; DestDir: "{app}\zyqs\images\gui"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\gui\setDlgSlider.fish"; DestDir: "{app}\zyqs\images\gui"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\gui\setDlgThumb.fish"; DestDir: "{app}\zyqs\images\gui"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\lock_fish\lock_flag.fish"; DestDir: "{app}\zyqs\images\lock_fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\lock_fish\lock_flag_1.fish"; DestDir: "{app}\zyqs\images\lock_fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\lock_fish\lock_flag_2.fish"; DestDir: "{app}\zyqs\images\lock_fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\lock_fish\lock_flag_3.fish"; DestDir: "{app}\zyqs\images\lock_fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\lock_fish\lock_flag_4.fish"; DestDir: "{app}\zyqs\images\lock_fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\lock_fish\lock_flag_5.fish"; DestDir: "{app}\zyqs\images\lock_fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\lock_fish\lock_flag_6.fish"; DestDir: "{app}\zyqs\images\lock_fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\lock_fish\lock_flag_7.fish"; DestDir: "{app}\zyqs\images\lock_fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\lock_fish\lock_flag_8.fish"; DestDir: "{app}\zyqs\images\lock_fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\lock_fish\lock_line.fish"; DestDir: "{app}\zyqs\images\lock_fish"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\net\net2.fish"; DestDir: "{app}\zyqs\images\net"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\net\net3.fish"; DestDir: "{app}\zyqs\images\net"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\net\net4.fish"; DestDir: "{app}\zyqs\images\net"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\prize\bingo.fish"; DestDir: "{app}\zyqs\images\prize"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\prize\bingo_num.fish"; DestDir: "{app}\zyqs\images\prize"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Scene\bg1.fish"; DestDir: "{app}\zyqs\images\Scene"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Scene\bg2.fish"; DestDir: "{app}\zyqs\images\Scene"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Scene\bg3.fish"; DestDir: "{app}\zyqs\images\Scene"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Scene\water1.fish"; DestDir: "{app}\zyqs\images\Scene"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Scene\water10.fish"; DestDir: "{app}\zyqs\images\Scene"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Scene\water11.fish"; DestDir: "{app}\zyqs\images\Scene"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Scene\water12.fish"; DestDir: "{app}\zyqs\images\Scene"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Scene\water13.fish"; DestDir: "{app}\zyqs\images\Scene"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Scene\water14.fish"; DestDir: "{app}\zyqs\images\Scene"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Scene\water15.fish"; DestDir: "{app}\zyqs\images\Scene"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Scene\water16.fish"; DestDir: "{app}\zyqs\images\Scene"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Scene\water2.fish"; DestDir: "{app}\zyqs\images\Scene"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Scene\water3.fish"; DestDir: "{app}\zyqs\images\Scene"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Scene\water4.fish"; DestDir: "{app}\zyqs\images\Scene"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Scene\water5.fish"; DestDir: "{app}\zyqs\images\Scene"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Scene\water6.fish"; DestDir: "{app}\zyqs\images\Scene"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Scene\water7.fish"; DestDir: "{app}\zyqs\images\Scene"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Scene\water8.fish"; DestDir: "{app}\zyqs\images\Scene"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Scene\water9.fish"; DestDir: "{app}\zyqs\images\Scene"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\images\Scene\wave.fish"; DestDir: "{app}\zyqs\images\Scene"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\particle\particle.fish"; DestDir: "{app}\zyqs\particle"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\particle\particle.ptc"; DestDir: "{app}\zyqs\particle"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\bgm\bgm1.fish"; DestDir: "{app}\zyqs\sounds\bgm"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\bgm\bgm2.fish"; DestDir: "{app}\zyqs\sounds\bgm"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\bgm\bgm3.fish"; DestDir: "{app}\zyqs\sounds\bgm"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\bgm\bgm4.fish"; DestDir: "{app}\zyqs\sounds\bgm"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\bingo.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\cannonSwitch.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\casting.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\catch.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\fire.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\fish10_1.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\fish10_2.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\fish11_1.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\fish11_2.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\fish12_1.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\fish12_2.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\fish13_1.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\fish13_2.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\fish14_1.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\fish14_2.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\fish15_1.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\fish15_2.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\fish16_1.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\fish16_2.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\fish17_1.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\fish17_2.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\fish17_3.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\gold.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\insert.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\ion_casting.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\ion_catch.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\ion_fire.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\ion_get.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\lock.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\silver.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\superarm.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 
Source: "{app}\zyqs\sounds\effect\wave.fish"; DestDir: "{app}\zyqs\sounds\effect"; MinVersion: 0.0,5.0; Flags: ignoreversion 

[Languages]
; These files are stubs
; To achieve better results after recompilation, use the real language files
Name: "default"; MessagesFile: "embedded\default.isl"; 
