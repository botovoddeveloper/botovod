object f: Tf
  Left = 0
  Top = 0
  BorderStyle = bsSizeToolWin
  Caption = ' ONLINE MAKER '
  ClientHeight = 282
  ClientWidth = 479
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel
    Left = 16
    Top = 37
    Width = 240
    Height = 13
    Caption = #1040#1076#1088#1077#1089' '#1092#1072#1081#1083#1072' "Upload.php" '#1085#1072' web '#1089#1077#1088#1074#1077#1088#1077':'
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Consolas'
    Font.Style = []
    ParentFont = False
  end
  object Edit2: TEdit
    Left = 16
    Top = 56
    Width = 441
    Height = 21
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Consolas'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    Text = 'Edit2'
    OnChange = Edit2Change
  end
  object b_DO: TButton
    Left = 16
    Top = 120
    Width = 441
    Height = 121
    Cursor = crHandPoint
    Caption = 
      #1057#1092#1086#1088#1084#1080#1088#1086#1074#1072#1090#1100' <SetOnline.php> '#1080#1079' '#1088#1086#1073#1086#1090#1086#1074#13#10#1074#1082#1083#1102#1095#1077#1085#1085#1099#1093' '#1075#1088#1091#1087#1087' '#1080' '#1074#1099#1075#1088 +
      #1091#1079#1080#1090#1100' '#1085#1072' web '#1089#1077#1088#1074#1077#1088
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Consolas'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    WordWrap = True
    OnClick = b_DOClick
  end
  object IdHTTP1: TIdHTTP
    AllowCookies = True
    ProxyParams.BasicAuthentication = False
    ProxyParams.ProxyPort = 0
    Request.ContentLength = -1
    Request.ContentRangeEnd = -1
    Request.ContentRangeStart = -1
    Request.ContentRangeInstanceLength = -1
    Request.Accept = 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8'
    Request.BasicAuthentication = False
    Request.UserAgent = 'Mozilla/3.0 (compatible; Indy Library)'
    Request.Ranges.Units = 'bytes'
    Request.Ranges = <>
    HTTPOptions = [hoForceEncodeParams]
    Left = 336
    Top = 8
  end
end
