#ifndef __STLWRT_TYPEDEFS_H__
#define __STLWRT_TYPEDEFS_H__

typedef struct _GdkAppLaunchContext GdkAppLaunchContext;
typedef struct _GdkColor GdkColor;
typedef struct _GdkColormap GdkColormap;
typedef struct _GdkCursor GdkCursor;
typedef struct _GdkDevice GdkDevice;
typedef struct _GdkDisplay GdkDisplay;
typedef struct _GdkDrawable GdkDrawable;
typedef struct _GdkFont GdkFont;
typedef struct _GdkGC GdkGC;
typedef struct _GdkGCValues GdkGCValues;
typedef struct _GdkRectangle GdkRectangle;
typedef struct _GdkScreen GdkScreen;
typedef struct _GdkVisual GdkVisual;
typedef struct _GtkAccelMap GtkAccelMap;
typedef struct _GtkAccessible GtkAccessible;
typedef struct _GtkActionGroup GtkActionGroup;
typedef struct _GtkAction GtkAction;
typedef struct _GtkActivatable GtkActivatable;
typedef struct _GtkBuildable GtkBuildable;
typedef struct _GtkBuilder GtkBuilder;
typedef struct _GtkCellEditable GtkCellEditable;
typedef struct _GtkCellLayout GtkCellLayout;
typedef struct _GtkClipboard GtkClipboard;
typedef struct _GtkEditable GtkEditable;
typedef struct _GtkEntryBuffer GtkEntryBuffer;
typedef struct _GtkEntryCompletion GtkEntryCompletion;
typedef struct _GtkFileChooserEmbed GtkFileChooserEmbed;
typedef struct _GtkFileChooserEntry GtkFileChooserEntry;
typedef struct _GtkFileChooser GtkFileChooser;
typedef struct _GtkFileChooserSettings GtkFileChooserSettings;
typedef struct _GtkFileFilter GtkFileFilter;
typedef struct _GtkFileSystem GtkFileSystem;
typedef struct _GtkFolder GtkFolder;
typedef struct _GtkFileSystemModel GtkFileSystemModel;
typedef struct _GtkIconSet GtkIconSet;
typedef struct _GtkIconSource GtkIconSource;
typedef struct _GtkIconFactory GtkIconFactory;
typedef struct _GtkIconInfo GtkIconInfo;
typedef struct _GtkIconTheme GtkIconTheme;
typedef struct _GtkIMContext GtkIMContext;
typedef struct _GtkMountOperation GtkMountOperation;
typedef struct _GtkOrientable GtkOrientable;
typedef struct _GtkPageSetup GtkPageSetup;
typedef struct _GtkPaperSize GtkPaperSize;
typedef struct _GtkPrintBackend GtkPrintBackend;
typedef struct _GtkPrintContext GtkPrintContext;
typedef struct _GtkPrinter GtkPrinter;
typedef struct _GtkPrintOperation GtkPrintOperation;
typedef struct _GtkPrintOperationPreview GtkPrintOperationPreview;
typedef struct _GtkPrintSettings GtkPrintSettings;
typedef struct _GtkQuery GtkQuery;
typedef struct _GtkRcProperty GtkRcProperty;
typedef struct _GtkRcStyle GtkRcStyle;
typedef struct _GtkRecentAction GtkRecentAction;
typedef struct _GtkRecentChooserDefault GtkRecentChooserDefault;
typedef struct _GtkRecentChooser GtkRecentChooser;
typedef struct _GtkRecentFilter GtkRecentFilter;
typedef struct _GtkRecentInfo GtkRecentInfo;
typedef struct _GtkRecentManager GtkRecentManager;
typedef struct _GtkSearchEngineBeagle GtkSearchEngineBeagle;
typedef struct _GtkSearchEngine GtkSearchEngine;
typedef struct _GtkSearchEngineSimple GtkSearchEngineSimple;
typedef struct _GtkSearchEngineTracker GtkSearchEngineTracker;
typedef struct _GtkSelectionData GtkSelectionData;
typedef struct _GtkTargetList GtkTargetList;
typedef struct _GtkStatusIcon GtkStatusIcon;
typedef struct _GtkStyle GtkStyle;
typedef struct _GtkBorder GtkBorder;
typedef struct _GtkTextChildAnchor GtkTextChildAnchor;
typedef struct _GtkTextIter GtkTextIter;
typedef struct _GtkTextMark GtkTextMark;
typedef struct _GtkThemeEngine GtkThemeEngine;
typedef struct _GtkToggleAction GtkToggleAction;
typedef struct _GtkToolShell GtkToolShell;
typedef struct _GtkTooltip GtkTooltip;
typedef struct _GtkTreeDragSource GtkTreeDragSource;
typedef struct _GtkTreeDragDest GtkTreeDragDest;
typedef struct _GtkTreeModelFilter GtkTreeModelFilter;
typedef struct _GtkTreeIter GtkTreeIter;
typedef struct _GtkTreeModel GtkTreeModel;
typedef struct _GtkTreeRowReference GtkTreeRowReference;
typedef struct _GtkTreePath GtkTreePath;
typedef struct _GtkTreeSortable GtkTreeSortable;
typedef struct _GtkIdentifier GtkIdentifier;
typedef struct _GtkUIManager GtkUIManager;
typedef struct _GtkRequisition GtkRequisition;
typedef struct _GtkWindowGroup GtkWindowGroup;

#ifdef STLWRT_COMPILATION

typedef struct _GtkAboutDialogFat GtkAboutDialogFat;
typedef struct _GtkAboutDialogThin GtkAboutDialogThin;
typedef union _GtkAboutDialog GtkAboutDialog;

typedef struct _GtkAccelGroupFat GtkAccelGroupFat;
typedef struct _GtkAccelGroupThin GtkAccelGroupThin;
typedef union _GtkAccelGroup GtkAccelGroup;

typedef struct _GtkAccelLabelFat GtkAccelLabelFat;
typedef struct _GtkAccelLabelThin GtkAccelLabelThin;
typedef union _GtkAccelLabel GtkAccelLabel;

typedef struct _GtkAdjustmentFat GtkAdjustmentFat;
typedef struct _GtkAdjustmentThin GtkAdjustmentThin;
typedef union _GtkAdjustment GtkAdjustment;

typedef struct _GtkAlignmentFat GtkAlignmentFat;
typedef struct _GtkAlignmentThin GtkAlignmentThin;
typedef union _GtkAlignment GtkAlignment;

typedef struct _GtkAssistantFat GtkAssistantFat;
typedef struct _GtkAssistantThin GtkAssistantThin;
typedef union _GtkAssistant GtkAssistant;

typedef struct _GtkBinFat GtkBinFat;
typedef struct _GtkBinThin GtkBinThin;
typedef union _GtkBin GtkBin;

typedef struct _GtkButtonBoxFat GtkButtonBoxFat;
typedef struct _GtkButtonBoxThin GtkButtonBoxThin;
typedef union _GtkButtonBox GtkButtonBox;

typedef struct _GtkBoxFat GtkBoxFat;
typedef struct _GtkBoxThin GtkBoxThin;
typedef union _GtkBox GtkBox;

typedef struct _GtkButtonFat GtkButtonFat;
typedef struct _GtkButtonThin GtkButtonThin;
typedef union _GtkButton GtkButton;

typedef struct _GtkCalendarFat GtkCalendarFat;
typedef struct _GtkCalendarThin GtkCalendarThin;
typedef union _GtkCalendar GtkCalendar;

typedef struct _GtkCellRendererAccelFat GtkCellRendererAccelFat;
typedef struct _GtkCellRendererAccelThin GtkCellRendererAccelThin;
typedef union _GtkCellRendererAccel GtkCellRendererAccel;

typedef struct _GtkCellRendererComboFat GtkCellRendererComboFat;
typedef struct _GtkCellRendererComboThin GtkCellRendererComboThin;
typedef union _GtkCellRendererCombo GtkCellRendererCombo;

typedef struct _GtkCellRendererFat GtkCellRendererFat;
typedef struct _GtkCellRendererThin GtkCellRendererThin;
typedef union _GtkCellRenderer GtkCellRenderer;

typedef struct _GtkCellRendererPixbufFat GtkCellRendererPixbufFat;
typedef struct _GtkCellRendererPixbufThin GtkCellRendererPixbufThin;
typedef union _GtkCellRendererPixbuf GtkCellRendererPixbuf;

typedef struct _GtkCellRendererProgressFat GtkCellRendererProgressFat;
typedef struct _GtkCellRendererProgressThin GtkCellRendererProgressThin;
typedef union _GtkCellRendererProgress GtkCellRendererProgress;

typedef struct _GtkCellRendererSpinFat GtkCellRendererSpinFat;
typedef struct _GtkCellRendererSpinThin GtkCellRendererSpinThin;
typedef union _GtkCellRendererSpin GtkCellRendererSpin;

typedef struct _GtkCellRendererSpinnerFat GtkCellRendererSpinnerFat;
typedef struct _GtkCellRendererSpinnerThin GtkCellRendererSpinnerThin;
typedef union _GtkCellRendererSpinner GtkCellRendererSpinner;

typedef struct _GtkCellRendererTextFat GtkCellRendererTextFat;
typedef struct _GtkCellRendererTextThin GtkCellRendererTextThin;
typedef union _GtkCellRendererText GtkCellRendererText;

typedef struct _GtkCellRendererToggleFat GtkCellRendererToggleFat;
typedef struct _GtkCellRendererToggleThin GtkCellRendererToggleThin;
typedef union _GtkCellRendererToggle GtkCellRendererToggle;

typedef struct _GtkCellViewFat GtkCellViewFat;
typedef struct _GtkCellViewThin GtkCellViewThin;
typedef union _GtkCellView GtkCellView;

typedef struct _GtkCheckButtonFat GtkCheckButtonFat;
typedef struct _GtkCheckButtonThin GtkCheckButtonThin;
typedef union _GtkCheckButton GtkCheckButton;

typedef struct _GtkCheckMenuItemFat GtkCheckMenuItemFat;
typedef struct _GtkCheckMenuItemThin GtkCheckMenuItemThin;
typedef union _GtkCheckMenuItem GtkCheckMenuItem;

typedef struct _GtkColorButtonFat GtkColorButtonFat;
typedef struct _GtkColorButtonThin GtkColorButtonThin;
typedef union _GtkColorButton GtkColorButton;

typedef struct _GtkColorSelectionDialogFat GtkColorSelectionDialogFat;
typedef struct _GtkColorSelectionDialogThin GtkColorSelectionDialogThin;
typedef union _GtkColorSelectionDialog GtkColorSelectionDialog;

typedef struct _GtkColorSelectionFat GtkColorSelectionFat;
typedef struct _GtkColorSelectionThin GtkColorSelectionThin;
typedef union _GtkColorSelection GtkColorSelection;

typedef struct _GtkComboBoxEntryFat GtkComboBoxEntryFat;
typedef struct _GtkComboBoxEntryThin GtkComboBoxEntryThin;
typedef union _GtkComboBoxEntry GtkComboBoxEntry;

typedef struct _GtkComboBoxFat GtkComboBoxFat;
typedef struct _GtkComboBoxThin GtkComboBoxThin;
typedef union _GtkComboBox GtkComboBox;

typedef struct _GtkComboBoxTextFat GtkComboBoxTextFat;
typedef struct _GtkComboBoxTextThin GtkComboBoxTextThin;
typedef union _GtkComboBoxText GtkComboBoxText;

typedef struct _GtkContainerFat GtkContainerFat;
typedef struct _GtkContainerThin GtkContainerThin;
typedef union _GtkContainer GtkContainer;

typedef struct _GtkCustomPaperUnixDialogFat GtkCustomPaperUnixDialogFat;
typedef struct _GtkCustomPaperUnixDialogThin GtkCustomPaperUnixDialogThin;
typedef union _GtkCustomPaperUnixDialog GtkCustomPaperUnixDialog;

typedef struct _GtkDialogFat GtkDialogFat;
typedef struct _GtkDialogThin GtkDialogThin;
typedef union _GtkDialog GtkDialog;

typedef struct _GtkDrawingAreaFat GtkDrawingAreaFat;
typedef struct _GtkDrawingAreaThin GtkDrawingAreaThin;
typedef union _GtkDrawingArea GtkDrawingArea;

typedef struct _GtkEntryFat GtkEntryFat;
typedef struct _GtkEntryThin GtkEntryThin;
typedef union _GtkEntry GtkEntry;

typedef struct _GtkEventBoxFat GtkEventBoxFat;
typedef struct _GtkEventBoxThin GtkEventBoxThin;
typedef union _GtkEventBox GtkEventBox;

typedef struct _GtkExpanderFat GtkExpanderFat;
typedef struct _GtkExpanderThin GtkExpanderThin;
typedef union _GtkExpander GtkExpander;

typedef struct _GtkFileChooserButtonFat GtkFileChooserButtonFat;
typedef struct _GtkFileChooserButtonThin GtkFileChooserButtonThin;
typedef union _GtkFileChooserButton GtkFileChooserButton;

typedef struct _GtkFileChooserDialogFat GtkFileChooserDialogFat;
typedef struct _GtkFileChooserDialogThin GtkFileChooserDialogThin;
typedef union _GtkFileChooserDialog GtkFileChooserDialog;

typedef struct _GtkFileChooserDefaultFat GtkFileChooserDefaultFat;
typedef struct _GtkFileChooserDefaultThin GtkFileChooserDefaultThin;
typedef union _GtkFileChooserDefault GtkFileChooserDefault;

typedef struct _GtkFileChooserWidgetFat GtkFileChooserWidgetFat;
typedef struct _GtkFileChooserWidgetThin GtkFileChooserWidgetThin;
typedef union _GtkFileChooserWidget GtkFileChooserWidget;

typedef struct _GtkFixedFat GtkFixedFat;
typedef struct _GtkFixedThin GtkFixedThin;
typedef union _GtkFixed GtkFixed;

typedef struct _GtkFontButtonFat GtkFontButtonFat;
typedef struct _GtkFontButtonThin GtkFontButtonThin;
typedef union _GtkFontButton GtkFontButton;

typedef struct _GtkFontSelectionFat GtkFontSelectionFat;
typedef struct _GtkFontSelectionThin GtkFontSelectionThin;
typedef union _GtkFontSelection GtkFontSelection;

typedef struct _GtkFontSelectionDialogFat GtkFontSelectionDialogFat;
typedef struct _GtkFontSelectionDialogThin GtkFontSelectionDialogThin;
typedef union _GtkFontSelectionDialog GtkFontSelectionDialog;

typedef struct _GtkFrameFat GtkFrameFat;
typedef struct _GtkFrameThin GtkFrameThin;
typedef union _GtkFrame GtkFrame;

typedef struct _GtkHandleBoxFat GtkHandleBoxFat;
typedef struct _GtkHandleBoxThin GtkHandleBoxThin;
typedef union _GtkHandleBox GtkHandleBox;

typedef struct _GtkHButtonBoxFat GtkHButtonBoxFat;
typedef struct _GtkHButtonBoxThin GtkHButtonBoxThin;
typedef union _GtkHButtonBox GtkHButtonBox;

typedef struct _GtkHBoxFat GtkHBoxFat;
typedef struct _GtkHBoxThin GtkHBoxThin;
typedef union _GtkHBox GtkHBox;

typedef struct _GtkHPanedFat GtkHPanedFat;
typedef struct _GtkHPanedThin GtkHPanedThin;
typedef union _GtkHPaned GtkHPaned;

typedef struct _GtkHRulerFat GtkHRulerFat;
typedef struct _GtkHRulerThin GtkHRulerThin;
typedef union _GtkHRuler GtkHRuler;

typedef struct _GtkHScaleFat GtkHScaleFat;
typedef struct _GtkHScaleThin GtkHScaleThin;
typedef union _GtkHScale GtkHScale;

typedef struct _GtkHScrollbarFat GtkHScrollbarFat;
typedef struct _GtkHScrollbarThin GtkHScrollbarThin;
typedef union _GtkHScrollbar GtkHScrollbar;

typedef struct _GtkHSeparatorFat GtkHSeparatorFat;
typedef struct _GtkHSeparatorThin GtkHSeparatorThin;
typedef union _GtkHSeparator GtkHSeparator;

typedef struct _GtkHSVFat GtkHSVFat;
typedef struct _GtkHSVThin GtkHSVThin;
typedef union _GtkHSV GtkHSV;

typedef struct _GtkIconViewFat GtkIconViewFat;
typedef struct _GtkIconViewThin GtkIconViewThin;
typedef union _GtkIconView GtkIconView;

typedef struct _GtkImageFat GtkImageFat;
typedef struct _GtkImageThin GtkImageThin;
typedef union _GtkImage GtkImage;

typedef struct _GtkImageMenuItemFat GtkImageMenuItemFat;
typedef struct _GtkImageMenuItemThin GtkImageMenuItemThin;
typedef union _GtkImageMenuItem GtkImageMenuItem;

typedef struct _GtkIMContextSimpleFat GtkIMContextSimpleFat;
typedef struct _GtkIMContextSimpleThin GtkIMContextSimpleThin;
typedef union _GtkIMContextSimple GtkIMContextSimple;

typedef struct _GtkIMMulticontextFat GtkIMMulticontextFat;
typedef struct _GtkIMMulticontextThin GtkIMMulticontextThin;
typedef union _GtkIMMulticontext GtkIMMulticontext;

typedef struct _GtkInfoBarFat GtkInfoBarFat;
typedef struct _GtkInfoBarThin GtkInfoBarThin;
typedef union _GtkInfoBar GtkInfoBar;

typedef struct _GtkInvisibleFat GtkInvisibleFat;
typedef struct _GtkInvisibleThin GtkInvisibleThin;
typedef union _GtkInvisible GtkInvisible;

typedef struct _GtkLabelFat GtkLabelFat;
typedef struct _GtkLabelThin GtkLabelThin;
typedef union _GtkLabel GtkLabel;

typedef struct _GtkLayoutFat GtkLayoutFat;
typedef struct _GtkLayoutThin GtkLayoutThin;
typedef union _GtkLayout GtkLayout;

typedef struct _GtkLinkButtonFat GtkLinkButtonFat;
typedef struct _GtkLinkButtonThin GtkLinkButtonThin;
typedef union _GtkLinkButton GtkLinkButton;

typedef struct _GtkListStoreFat GtkListStoreFat;
typedef struct _GtkListStoreThin GtkListStoreThin;
typedef union _GtkListStore GtkListStore;

typedef struct _GtkMenuBarFat GtkMenuBarFat;
typedef struct _GtkMenuBarThin GtkMenuBarThin;
typedef union _GtkMenuBar GtkMenuBar;

typedef struct _GtkMenuFat GtkMenuFat;
typedef struct _GtkMenuThin GtkMenuThin;
typedef union _GtkMenu GtkMenu;

typedef struct _GtkMenuItemFat GtkMenuItemFat;
typedef struct _GtkMenuItemThin GtkMenuItemThin;
typedef union _GtkMenuItem GtkMenuItem;

typedef struct _GtkMenuShellFat GtkMenuShellFat;
typedef struct _GtkMenuShellThin GtkMenuShellThin;
typedef union _GtkMenuShell GtkMenuShell;

typedef struct _GtkMenuToolButtonFat GtkMenuToolButtonFat;
typedef struct _GtkMenuToolButtonThin GtkMenuToolButtonThin;
typedef union _GtkMenuToolButton GtkMenuToolButton;

typedef struct _GtkMessageDialogFat GtkMessageDialogFat;
typedef struct _GtkMessageDialogThin GtkMessageDialogThin;
typedef union _GtkMessageDialog GtkMessageDialog;

typedef struct _GtkMiscFat GtkMiscFat;
typedef struct _GtkMiscThin GtkMiscThin;
typedef union _GtkMisc GtkMisc;

typedef struct _GtkNotebookFat GtkNotebookFat;
typedef struct _GtkNotebookThin GtkNotebookThin;
typedef union _GtkNotebook GtkNotebook;

typedef struct _GtkOffscreenWindowFat GtkOffscreenWindowFat;
typedef struct _GtkOffscreenWindowThin GtkOffscreenWindowThin;
typedef union _GtkOffscreenWindow GtkOffscreenWindow;

typedef struct _GtkPageSetupUnixDialogFat GtkPageSetupUnixDialogFat;
typedef struct _GtkPageSetupUnixDialogThin GtkPageSetupUnixDialogThin;
typedef union _GtkPageSetupUnixDialog GtkPageSetupUnixDialog;

typedef struct _GtkPanedFat GtkPanedFat;
typedef struct _GtkPanedThin GtkPanedThin;
typedef union _GtkPaned GtkPaned;

typedef struct _GtkPathBarFat GtkPathBarFat;
typedef struct _GtkPathBarThin GtkPathBarThin;
typedef union _GtkPathBar GtkPathBar;

typedef struct _GtkPlugFat GtkPlugFat;
typedef struct _GtkPlugThin GtkPlugThin;
typedef union _GtkPlug GtkPlug;

typedef struct _GtkPrinterOptionFat GtkPrinterOptionFat;
typedef struct _GtkPrinterOptionThin GtkPrinterOptionThin;
typedef union _GtkPrinterOption GtkPrinterOption;

typedef struct _GtkPrinterOptionSetFat GtkPrinterOptionSetFat;
typedef struct _GtkPrinterOptionSetThin GtkPrinterOptionSetThin;
typedef union _GtkPrinterOptionSet GtkPrinterOptionSet;

typedef struct _GtkPrinterOptionWidgetFat GtkPrinterOptionWidgetFat;
typedef struct _GtkPrinterOptionWidgetThin GtkPrinterOptionWidgetThin;
typedef union _GtkPrinterOptionWidget GtkPrinterOptionWidget;

typedef struct _GtkPrintJobFat GtkPrintJobFat;
typedef struct _GtkPrintJobThin GtkPrintJobThin;
typedef union _GtkPrintJob GtkPrintJob;

typedef struct _GtkPrintUnixDialogFat GtkPrintUnixDialogFat;
typedef struct _GtkPrintUnixDialogThin GtkPrintUnixDialogThin;
typedef union _GtkPrintUnixDialog GtkPrintUnixDialog;

typedef struct _GtkProgressBarFat GtkProgressBarFat;
typedef struct _GtkProgressBarThin GtkProgressBarThin;
typedef union _GtkProgressBar GtkProgressBar;

typedef struct _GtkRadioActionFat GtkRadioActionFat;
typedef struct _GtkRadioActionThin GtkRadioActionThin;
typedef union _GtkRadioAction GtkRadioAction;

typedef struct _GtkRadioButtonFat GtkRadioButtonFat;
typedef struct _GtkRadioButtonThin GtkRadioButtonThin;
typedef union _GtkRadioButton GtkRadioButton;

typedef struct _GtkRadioMenuItemFat GtkRadioMenuItemFat;
typedef struct _GtkRadioMenuItemThin GtkRadioMenuItemThin;
typedef union _GtkRadioMenuItem GtkRadioMenuItem;

typedef struct _GtkRadioToolButtonFat GtkRadioToolButtonFat;
typedef struct _GtkRadioToolButtonThin GtkRadioToolButtonThin;
typedef union _GtkRadioToolButton GtkRadioToolButton;

typedef struct _GtkRangeFat GtkRangeFat;
typedef struct _GtkRangeThin GtkRangeThin;
typedef union _GtkRange GtkRange;

typedef struct _GtkRecentChooserDialogFat GtkRecentChooserDialogFat;
typedef struct _GtkRecentChooserDialogThin GtkRecentChooserDialogThin;
typedef union _GtkRecentChooserDialog GtkRecentChooserDialog;

typedef struct _GtkRecentChooserMenuFat GtkRecentChooserMenuFat;
typedef struct _GtkRecentChooserMenuThin GtkRecentChooserMenuThin;
typedef union _GtkRecentChooserMenu GtkRecentChooserMenu;

typedef struct _GtkRecentChooserWidgetFat GtkRecentChooserWidgetFat;
typedef struct _GtkRecentChooserWidgetThin GtkRecentChooserWidgetThin;
typedef union _GtkRecentChooserWidget GtkRecentChooserWidget;

typedef struct _GtkRulerFat GtkRulerFat;
typedef struct _GtkRulerThin GtkRulerThin;
typedef union _GtkRuler GtkRuler;

typedef struct _GtkScaleButtonFat GtkScaleButtonFat;
typedef struct _GtkScaleButtonThin GtkScaleButtonThin;
typedef union _GtkScaleButton GtkScaleButton;

typedef struct _GtkScaleFat GtkScaleFat;
typedef struct _GtkScaleThin GtkScaleThin;
typedef union _GtkScale GtkScale;

typedef struct _GtkScrollbarFat GtkScrollbarFat;
typedef struct _GtkScrollbarThin GtkScrollbarThin;
typedef union _GtkScrollbar GtkScrollbar;

typedef struct _GtkScrolledWindowFat GtkScrolledWindowFat;
typedef struct _GtkScrolledWindowThin GtkScrolledWindowThin;
typedef union _GtkScrolledWindow GtkScrolledWindow;

typedef struct _GtkSeparatorFat GtkSeparatorFat;
typedef struct _GtkSeparatorThin GtkSeparatorThin;
typedef union _GtkSeparator GtkSeparator;

typedef struct _GtkSeparatorMenuItemFat GtkSeparatorMenuItemFat;
typedef struct _GtkSeparatorMenuItemThin GtkSeparatorMenuItemThin;
typedef union _GtkSeparatorMenuItem GtkSeparatorMenuItem;

typedef struct _GtkSeparatorToolItemFat GtkSeparatorToolItemFat;
typedef struct _GtkSeparatorToolItemThin GtkSeparatorToolItemThin;
typedef union _GtkSeparatorToolItem GtkSeparatorToolItem;

typedef struct _GtkSettingsFat GtkSettingsFat;
typedef struct _GtkSettingsThin GtkSettingsThin;
typedef union _GtkSettings GtkSettings;

typedef struct _GtkSizeGroupFat GtkSizeGroupFat;
typedef struct _GtkSizeGroupThin GtkSizeGroupThin;
typedef union _GtkSizeGroup GtkSizeGroup;

typedef struct _GtkSocketFat GtkSocketFat;
typedef struct _GtkSocketThin GtkSocketThin;
typedef union _GtkSocket GtkSocket;

typedef struct _GtkSpinButtonFat GtkSpinButtonFat;
typedef struct _GtkSpinButtonThin GtkSpinButtonThin;
typedef union _GtkSpinButton GtkSpinButton;

typedef struct _GtkSpinnerFat GtkSpinnerFat;
typedef struct _GtkSpinnerThin GtkSpinnerThin;
typedef union _GtkSpinner GtkSpinner;

typedef struct _GtkStatusbarFat GtkStatusbarFat;
typedef struct _GtkStatusbarThin GtkStatusbarThin;
typedef union _GtkStatusbar GtkStatusbar;

typedef struct _GtkTableFat GtkTableFat;
typedef struct _GtkTableThin GtkTableThin;
typedef union _GtkTable GtkTable;

typedef struct _GtkTearoffMenuItemFat GtkTearoffMenuItemFat;
typedef struct _GtkTearoffMenuItemThin GtkTearoffMenuItemThin;
typedef union _GtkTearoffMenuItem GtkTearoffMenuItem;

typedef struct _GtkTextBufferFat GtkTextBufferFat;
typedef struct _GtkTextBufferThin GtkTextBufferThin;
typedef union _GtkTextBuffer GtkTextBuffer;

typedef struct _GtkTextLayoutFat GtkTextLayoutFat;
typedef struct _GtkTextLayoutThin GtkTextLayoutThin;
typedef union _GtkTextLayout GtkTextLayout;

typedef struct _GtkTextTagFat GtkTextTagFat;
typedef struct _GtkTextTagThin GtkTextTagThin;
typedef union _GtkTextTag GtkTextTag;

typedef struct _GtkTextTagTableFat GtkTextTagTableFat;
typedef struct _GtkTextTagTableThin GtkTextTagTableThin;
typedef union _GtkTextTagTable GtkTextTagTable;

typedef struct _GtkTextViewFat GtkTextViewFat;
typedef struct _GtkTextViewThin GtkTextViewThin;
typedef union _GtkTextView GtkTextView;

typedef struct _GtkToggleButtonFat GtkToggleButtonFat;
typedef struct _GtkToggleButtonThin GtkToggleButtonThin;
typedef union _GtkToggleButton GtkToggleButton;

typedef struct _GtkToggleToolButtonFat GtkToggleToolButtonFat;
typedef struct _GtkToggleToolButtonThin GtkToggleToolButtonThin;
typedef union _GtkToggleToolButton GtkToggleToolButton;

typedef struct _GtkToolbarFat GtkToolbarFat;
typedef struct _GtkToolbarThin GtkToolbarThin;
typedef union _GtkToolbar GtkToolbar;

typedef struct _GtkToolButtonFat GtkToolButtonFat;
typedef struct _GtkToolButtonThin GtkToolButtonThin;
typedef union _GtkToolButton GtkToolButton;

typedef struct _GtkToolItemGroupFat GtkToolItemGroupFat;
typedef struct _GtkToolItemGroupThin GtkToolItemGroupThin;
typedef union _GtkToolItemGroup GtkToolItemGroup;

typedef struct _GtkToolItemFat GtkToolItemFat;
typedef struct _GtkToolItemThin GtkToolItemThin;
typedef union _GtkToolItem GtkToolItem;

typedef struct _GtkToolPaletteFat GtkToolPaletteFat;
typedef struct _GtkToolPaletteThin GtkToolPaletteThin;
typedef union _GtkToolPalette GtkToolPalette;

typedef struct _GtkTrayIconFat GtkTrayIconFat;
typedef struct _GtkTrayIconThin GtkTrayIconThin;
typedef union _GtkTrayIcon GtkTrayIcon;

typedef struct _GtkTreeModelSortFat GtkTreeModelSortFat;
typedef struct _GtkTreeModelSortThin GtkTreeModelSortThin;
typedef union _GtkTreeModelSort GtkTreeModelSort;

typedef struct _GtkTreeSelectionFat GtkTreeSelectionFat;
typedef struct _GtkTreeSelectionThin GtkTreeSelectionThin;
typedef union _GtkTreeSelection GtkTreeSelection;

typedef struct _GtkTreeStoreFat GtkTreeStoreFat;
typedef struct _GtkTreeStoreThin GtkTreeStoreThin;
typedef union _GtkTreeStore GtkTreeStore;

typedef struct _GtkTreeViewColumnFat GtkTreeViewColumnFat;
typedef struct _GtkTreeViewColumnThin GtkTreeViewColumnThin;
typedef union _GtkTreeViewColumn GtkTreeViewColumn;

typedef struct _GtkTreeViewFat GtkTreeViewFat;
typedef struct _GtkTreeViewThin GtkTreeViewThin;
typedef union _GtkTreeView GtkTreeView;

typedef struct _GtkVButtonBoxFat GtkVButtonBoxFat;
typedef struct _GtkVButtonBoxThin GtkVButtonBoxThin;
typedef union _GtkVButtonBox GtkVButtonBox;

typedef struct _GtkVBoxFat GtkVBoxFat;
typedef struct _GtkVBoxThin GtkVBoxThin;
typedef union _GtkVBox GtkVBox;

typedef struct _GtkViewportFat GtkViewportFat;
typedef struct _GtkViewportThin GtkViewportThin;
typedef union _GtkViewport GtkViewport;

typedef struct _GtkVolumeButtonFat GtkVolumeButtonFat;
typedef struct _GtkVolumeButtonThin GtkVolumeButtonThin;
typedef union _GtkVolumeButton GtkVolumeButton;

typedef struct _GtkVPanedFat GtkVPanedFat;
typedef struct _GtkVPanedThin GtkVPanedThin;
typedef union _GtkVPaned GtkVPaned;

typedef struct _GtkVRulerFat GtkVRulerFat;
typedef struct _GtkVRulerThin GtkVRulerThin;
typedef union _GtkVRuler GtkVRuler;

typedef struct _GtkVScaleFat GtkVScaleFat;
typedef struct _GtkVScaleThin GtkVScaleThin;
typedef union _GtkVScale GtkVScale;

typedef struct _GtkVScrollbarFat GtkVScrollbarFat;
typedef struct _GtkVScrollbarThin GtkVScrollbarThin;
typedef union _GtkVScrollbar GtkVScrollbar;

typedef struct _GtkVSeparatorFat GtkVSeparatorFat;
typedef struct _GtkVSeparatorThin GtkVSeparatorThin;
typedef union _GtkVSeparator GtkVSeparator;

typedef struct _GtkWidgetFat GtkWidgetFat;
typedef struct _GtkWidgetThin GtkWidgetThin;
typedef union _GtkWidget GtkWidget;

typedef struct _GtkWindowFat GtkWindowFat;
typedef struct _GtkWindowThin GtkWindowThin;
typedef union _GtkWindow GtkWindow;

#else

typedef struct _GtkAboutDialog GtkAboutDialog;
typedef struct _GtkAccelGroup GtkAccelGroup;
typedef struct _GtkAccelLabel GtkAccelLabel;
typedef struct _GtkAdjustment GtkAdjustment;
typedef struct _GtkAlignment GtkAlignment;
typedef struct _GtkAssistant GtkAssistant;
typedef struct _GtkBin GtkBin;
typedef struct _GtkButtonBox GtkButtonBox;
typedef struct _GtkBox GtkBox;
typedef struct _GtkButton GtkButton;
typedef struct _GtkCalendar GtkCalendar;
typedef struct _GtkCellRendererAccel GtkCellRendererAccel;
typedef struct _GtkCellRendererCombo GtkCellRendererCombo;
typedef struct _GtkCellRenderer GtkCellRenderer;
typedef struct _GtkCellRendererPixbuf GtkCellRendererPixbuf;
typedef struct _GtkCellRendererProgress GtkCellRendererProgress;
typedef struct _GtkCellRendererSpin GtkCellRendererSpin;
typedef struct _GtkCellRendererSpinner GtkCellRendererSpinner;
typedef struct _GtkCellRendererText GtkCellRendererText;
typedef struct _GtkCellRendererToggle GtkCellRendererToggle;
typedef struct _GtkCellView GtkCellView;
typedef struct _GtkCheckButton GtkCheckButton;
typedef struct _GtkCheckMenuItem GtkCheckMenuItem;
typedef struct _GtkColorButton GtkColorButton;
typedef struct _GtkColorSelectionDialog GtkColorSelectionDialog;
typedef struct _GtkColorSelection GtkColorSelection;
typedef struct _GtkComboBoxEntry GtkComboBoxEntry;
typedef struct _GtkComboBox GtkComboBox;
typedef struct _GtkComboBoxText GtkComboBoxText;
typedef struct _GtkContainer GtkContainer;
typedef struct _GtkCustomPaperUnixDialog GtkCustomPaperUnixDialog;
typedef struct _GtkDialog GtkDialog;
typedef struct _GtkDrawingArea GtkDrawingArea;
typedef struct _GtkEntry GtkEntry;
typedef struct _GtkEventBox GtkEventBox;
typedef struct _GtkExpander GtkExpander;
typedef struct _GtkFileChooserButton GtkFileChooserButton;
typedef struct _GtkFileChooserDialog GtkFileChooserDialog;
typedef struct _GtkFileChooserDefault GtkFileChooserDefault;
typedef struct _GtkFileChooserWidget GtkFileChooserWidget;
typedef struct _GtkFixed GtkFixed;
typedef struct _GtkFontButton GtkFontButton;
typedef struct _GtkFontSelection GtkFontSelection;
typedef struct _GtkFontSelectionDialog GtkFontSelectionDialog;
typedef struct _GtkFrame GtkFrame;
typedef struct _GtkHandleBox GtkHandleBox;
typedef struct _GtkHButtonBox GtkHButtonBox;
typedef struct _GtkHBox GtkHBox;
typedef struct _GtkHPaned GtkHPaned;
typedef struct _GtkHRuler GtkHRuler;
typedef struct _GtkHScale GtkHScale;
typedef struct _GtkHScrollbar GtkHScrollbar;
typedef struct _GtkHSeparator GtkHSeparator;
typedef struct _GtkHSV GtkHSV;
typedef struct _GtkIconView GtkIconView;
typedef struct _GtkImage GtkImage;
typedef struct _GtkImageMenuItem GtkImageMenuItem;
typedef struct _GtkIMContextSimple GtkIMContextSimple;
typedef struct _GtkIMMulticontext GtkIMMulticontext;
typedef struct _GtkInfoBar GtkInfoBar;
typedef struct _GtkInvisible GtkInvisible;
typedef struct _GtkLabel GtkLabel;
typedef struct _GtkLayout GtkLayout;
typedef struct _GtkLinkButton GtkLinkButton;
typedef struct _GtkListStore GtkListStore;
typedef struct _GtkMenuBar GtkMenuBar;
typedef struct _GtkMenu GtkMenu;
typedef struct _GtkMenuItem GtkMenuItem;
typedef struct _GtkMenuShell GtkMenuShell;
typedef struct _GtkMenuToolButton GtkMenuToolButton;
typedef struct _GtkMessageDialog GtkMessageDialog;
typedef struct _GtkMisc GtkMisc;
typedef struct _GtkNotebook GtkNotebook;
typedef struct _GtkOffscreenWindow GtkOffscreenWindow;
typedef struct _GtkPageSetupUnixDialog GtkPageSetupUnixDialog;
typedef struct _GtkPaned GtkPaned;
typedef struct _GtkPathBar GtkPathBar;
typedef struct _GtkPlug GtkPlug;
typedef struct _GtkPrinterOption GtkPrinterOption;
typedef struct _GtkPrinterOptionSet GtkPrinterOptionSet;
typedef struct _GtkPrinterOptionWidget GtkPrinterOptionWidget;
typedef struct _GtkPrintJob GtkPrintJob;
typedef struct _GtkPrintUnixDialog GtkPrintUnixDialog;
typedef struct _GtkProgressBar GtkProgressBar;
typedef struct _GtkRadioAction GtkRadioAction;
typedef struct _GtkRadioButton GtkRadioButton;
typedef struct _GtkRadioMenuItem GtkRadioMenuItem;
typedef struct _GtkRadioToolButton GtkRadioToolButton;
typedef struct _GtkRange GtkRange;
typedef struct _GtkRecentChooserDialog GtkRecentChooserDialog;
typedef struct _GtkRecentChooserMenu GtkRecentChooserMenu;
typedef struct _GtkRecentChooserWidget GtkRecentChooserWidget;
typedef struct _GtkRuler GtkRuler;
typedef struct _GtkScaleButton GtkScaleButton;
typedef struct _GtkScale GtkScale;
typedef struct _GtkScrollbar GtkScrollbar;
typedef struct _GtkScrolledWindow GtkScrolledWindow;
typedef struct _GtkSeparator GtkSeparator;
typedef struct _GtkSeparatorMenuItem GtkSeparatorMenuItem;
typedef struct _GtkSeparatorToolItem GtkSeparatorToolItem;
typedef struct _GtkSettings GtkSettings;
typedef struct _GtkSizeGroup GtkSizeGroup;
typedef struct _GtkSocket GtkSocket;
typedef struct _GtkSpinButton GtkSpinButton;
typedef struct _GtkSpinner GtkSpinner;
typedef struct _GtkStatusbar GtkStatusbar;
typedef struct _GtkTable GtkTable;
typedef struct _GtkTearoffMenuItem GtkTearoffMenuItem;
typedef struct _GtkTextBuffer GtkTextBuffer;
typedef struct _GtkTextLayout GtkTextLayout;
typedef struct _GtkTextTag GtkTextTag;
typedef struct _GtkTextTagTable GtkTextTagTable;
typedef struct _GtkTextView GtkTextView;
typedef struct _GtkToggleButton GtkToggleButton;
typedef struct _GtkToggleToolButton GtkToggleToolButton;
typedef struct _GtkToolbar GtkToolbar;
typedef struct _GtkToolButton GtkToolButton;
typedef struct _GtkToolItemGroup GtkToolItemGroup;
typedef struct _GtkToolItem GtkToolItem;
typedef struct _GtkToolPalette GtkToolPalette;
typedef struct _GtkTrayIcon GtkTrayIcon;
typedef struct _GtkTreeModelSort GtkTreeModelSort;
typedef struct _GtkTreeSelection GtkTreeSelection;
typedef struct _GtkTreeStore GtkTreeStore;
typedef struct _GtkTreeViewColumn GtkTreeViewColumn;
typedef struct _GtkTreeView GtkTreeView;
typedef struct _GtkVButtonBox GtkVButtonBox;
typedef struct _GtkVBox GtkVBox;
typedef struct _GtkViewport GtkViewport;
typedef struct _GtkVolumeButton GtkVolumeButton;
typedef struct _GtkVPaned GtkVPaned;
typedef struct _GtkVRuler GtkVRuler;
typedef struct _GtkVScale GtkVScale;
typedef struct _GtkVScrollbar GtkVScrollbar;
typedef struct _GtkVSeparator GtkVSeparator;
typedef struct _GtkWidget GtkWidget;
typedef struct _GtkWindow GtkWindow;

#endif

typedef struct _GdkEventAny	    GdkEventAny;
typedef struct _GdkEventExpose	    GdkEventExpose;
typedef struct _GdkEventNoExpose    GdkEventNoExpose;
typedef struct _GdkEventVisibility  GdkEventVisibility;
typedef struct _GdkEventMotion	    GdkEventMotion;
typedef struct _GdkEventButton	    GdkEventButton;
typedef struct _GdkEventScroll      GdkEventScroll;  
typedef struct _GdkEventKey	    GdkEventKey;
typedef struct _GdkEventFocus	    GdkEventFocus;
typedef struct _GdkEventCrossing    GdkEventCrossing;
typedef struct _GdkEventConfigure   GdkEventConfigure;
typedef struct _GdkEventProperty    GdkEventProperty;
typedef struct _GdkEventSelection   GdkEventSelection;
typedef struct _GdkEventOwnerChange GdkEventOwnerChange;
typedef struct _GdkEventProximity   GdkEventProximity;
typedef struct _GdkEventClient	    GdkEventClient;
typedef struct _GdkEventDND         GdkEventDND;
typedef struct _GdkEventWindowState GdkEventWindowState;
typedef struct _GdkEventSetting     GdkEventSetting;
typedef struct _GdkEventGrabBroken  GdkEventGrabBroken;
typedef union  _GdkEvent	    GdkEvent;


typedef struct _GdkWindow GdkWindow;
typedef struct _GdkRegion GdkRegion;
typedef struct _GdkImage GdkImage;
typedef struct _GdkDragContext GdkDragContext;
typedef struct _GdkPixmap GdkPixmap;
typedef struct _GdkDrawable GdkBitmap;
typedef struct _GdkDisplayManager GdkDisplayManager;
typedef struct _GdkPangoRenderer GdkPangoRenderer;

typedef enum _GdkGCValuesMask GdkGCValuesMask;

#endif /* __STLWRT_TYPEDEFS_H__ */
