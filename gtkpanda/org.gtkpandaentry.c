/* GTK - The GIMP Toolkit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 * Modified by the GTK+ Team and others 1997-1999.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
 */

//#define USE_XIM

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtkentry.h>
#include <gtk/gtksignal.h>
#include <gdk/gdkprivate.h>
#include "gdk/gdkkeysyms.h"
#include <X11/Xlib.h>
#include "gtkpandaintl.h"
#include "gtkpandaentry.h"

static void gtk_panda_entry_class_init    (GtkPandaEntryClass *klass);
static void gtk_panda_entry_init          (GtkPandaEntry     *entry);

//static void gtk_panda_entry_realize       (GtkWidget         *widget);
static void gtk_panda_entry_unrealize     (GtkWidget         *widget);
static void gtk_panda_entry_hide          (GtkWidget         *widget);
static gint gtk_panda_entry_button_press  (GtkWidget         *widget,
					   GdkEventButton    *event);
static gint gtk_panda_entry_focus_in      (GtkWidget         *widget,
					   GdkEventFocus     *event);
static gint gtk_panda_entry_focus_out     (GtkWidget         *widget,
					   GdkEventFocus     *event);
static gint gtk_panda_entry_key_press     (GtkWidget         *widget,
					   GdkEventKey       *event);
static void gtk_panda_entry_insert_text   (GtkEditable       *editable,
					   const gchar       *new_text,
					   gint               new_text_length,
					   gint              *position);

static GtkEntryClass *parent_class = NULL;

GtkType
gtk_panda_entry_get_type (void)
{
  static GtkType type = 0;

  if (!type)
    {
      static const GtkTypeInfo info =
      {
	"GtkPandaEntry",
	sizeof (GtkPandaEntry),
	sizeof (GtkPandaEntryClass),
	(GtkClassInitFunc) gtk_panda_entry_class_init,
	(GtkObjectInitFunc) gtk_panda_entry_init,
	/* reserved_1 */ NULL,
	/* reserved_2 */ NULL,
        (GtkClassInitFunc) NULL,
      };

      type = gtk_type_unique (GTK_TYPE_ENTRY, &info);
    }

  return type;
}

static void
gtk_panda_entry_class_init (GtkPandaEntryClass *class)
{
  GtkObjectClass *object_class;
  GtkWidgetClass *widget_class;
  GtkEditableClass *editable_class;
  GtkEntryClass *entry_class;

  object_class = (GtkObjectClass*) class;
  widget_class = (GtkWidgetClass*) class;
  editable_class = (GtkEditableClass*) class;
  entry_class = (GtkEntryClass*) class;
  parent_class = gtk_type_class (GTK_TYPE_ENTRY);

  //widget_class->realize = gtk_panda_entry_realize;
  widget_class->unrealize = gtk_panda_entry_unrealize;
  widget_class->hide = gtk_panda_entry_hide;
  widget_class->button_press_event = gtk_panda_entry_button_press;
  widget_class->focus_in_event = gtk_panda_entry_focus_in;
  widget_class->focus_out_event = gtk_panda_entry_focus_out;

  editable_class->insert_text = gtk_panda_entry_insert_text;
}

static void
gtk_panda_entry_init (GtkPandaEntry *entry)
{
  entry->input_mode = GTK_PANDA_ENTRY_XIM_MODE;
  entry->xim_enabled = FALSE;
  gtk_signal_connect(GTK_OBJECT (entry), "key_press_event",
					 (GtkSignalFunc) gtk_panda_entry_key_press, entry);
}

GtkWidget*
gtk_panda_entry_new (void)
{
  //return GTK_WIDGET (gtk_type_new (GTK_PANDA_TYPE_ENTRY));
  return g_object_new (GTK_PANDA_TYPE_ENTRY, NULL);
}

void
gtk_panda_entry_set_input_mode (GtkPandaEntry *entry,
				enum gtk_panda_entry_input_mode mode)
{
  entry->input_mode = mode;
}

void
gtk_panda_entry_set_xim_enabled (GtkPandaEntry *entry, gboolean flag)
{
  entry->xim_enabled = flag;
}

/* Recompute the x offsets of all characters in the buffer */
//static void
//gtk_entry_recompute_offsets (GtkEntry *entry)
//{
//  gint i;
//  gint offset = 0;
//  GtkEditable *editable = GTK_EDITABLE (entry);
//
//  for (i=0; i<entry->text_length; i++)
//    {
//      GdkWChar ch;
//
//      entry->char_offset[i] = offset;
//
//      if (editable->visible)
//	ch = entry->text[i];
//      else
//	ch = gtk_entry_get_invisible_char (entry);
//
//      if (entry->use_wchar)
//	offset += gdk_char_width_wc (GTK_WIDGET (entry)->style->font, ch);
//      else
//	offset += gdk_char_width (GTK_WIDGET (entry)->style->font, ch);
//    }
//  
//  entry->char_offset[i] = offset;
//}

//static void
//gtk_panda_entry_realize (GtkWidget *widget)
//{
//  GtkEntry *entry;
//  GtkEditable *editable;
//  GtkRequisition requisition;
//  GdkWindowAttr attributes;
//  gint attributes_mask;
//
//  g_return_if_fail (widget != NULL);
//  g_return_if_fail (GTK_IS_ENTRY (widget));
//
//  GTK_WIDGET_SET_FLAGS (widget, GTK_REALIZED);
//  entry = GTK_ENTRY (widget);
//  editable = GTK_EDITABLE (widget);
//
//  gtk_widget_get_child_requisition (widget, &requisition);
//  
//  attributes.window_type = GDK_WINDOW_CHILD;
//  attributes.x = widget->allocation.x;
//  attributes.y = widget->allocation.y + (widget->allocation.height -
//					 requisition.height) / 2;
//  attributes.width = widget->allocation.width;
//  attributes.height = requisition.height;
//  attributes.wclass = GDK_INPUT_OUTPUT;
//  attributes.visual = gtk_widget_get_visual (widget);
//  attributes.colormap = gtk_widget_get_colormap (widget);
//  attributes.event_mask = gtk_widget_get_events (widget);
//  attributes.event_mask |= (GDK_EXPOSURE_MASK |
//			    GDK_BUTTON_PRESS_MASK |
//			    GDK_BUTTON_RELEASE_MASK |
//			    GDK_BUTTON1_MOTION_MASK |
//			    GDK_BUTTON3_MOTION_MASK |
//			    GDK_POINTER_MOTION_HINT_MASK |
//			    GDK_ENTER_NOTIFY_MASK |
//			    GDK_LEAVE_NOTIFY_MASK |
//			    GDK_KEY_PRESS_MASK);
//  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;
//
//  widget->window = gdk_window_new (gtk_widget_get_parent_window (widget), &attributes, attributes_mask);
//  gdk_window_set_user_data (widget->window, entry);
//
//  get_text_area_size (entry, &attributes.x, &attributes.y, &attributes.width, &attributes.height);
//
//  attributes.cursor = gdk_cursor_new_for_display (gtk_widget_get_display (widget), GDK_XTERM);
//
//  attributes_mask |= GDK_WA_CURSOR;
//
//  entry->text_area = gdk_window_new (widget->window, &attributes, attributes_mask);
//  gdk_window_set_user_data (entry->text_area, entry);
//
//  gdk_cursor_unref (attributes.cursor);
//
//  widget->style = gtk_style_attach (widget->style, widget->window);
//
//  gdk_window_set_background (widget->window, &widget->style->base[GTK_WIDGET_STATE (widget)]);
//  gdk_window_set_background (entry->text_area, &widget->style->base[GTK_WIDGET_STATE (widget)]);
//
//  gdk_window_show (entry->text_area);
//
//  gtk_im_context_set_client_window (entry->im_context, entry->text_area); 
//  gtk_entry_adjust_scroll (entry);
//  gtk_entry_update_primary_selection (entry);
//}

static void
gtk_panda_entry_unrealize (GtkWidget *widget)
{
  g_return_if_fail (widget != NULL);
  g_return_if_fail (GTK_IS_ENTRY (widget));

  if (GTK_WIDGET_CLASS (parent_class)->unrealize)
    (* GTK_WIDGET_CLASS (parent_class)->unrealize) (widget);
}

static void
gtk_panda_entry_hide (GtkWidget *widget)
{
  g_return_if_fail (widget != NULL);
  g_return_if_fail (GTK_IS_WIDGET (widget));

  if (GTK_WIDGET_CLASS (parent_class)->hide)
    (* GTK_WIDGET_CLASS (parent_class)->hide) (widget);

#ifdef USE_XIM
  if ( (GTK_EDITABLE (widget))->ic)
    {
      gdk_ic_destroy ((GTK_EDITABLE (widget))->ic);
      (GTK_EDITABLE (widget))->ic = NULL;
    }
  if ((GTK_EDITABLE (widget))->ic_attr)
    {
      gdk_ic_attr_destroy ((GTK_EDITABLE (widget))->ic_attr);
      (GTK_EDITABLE (widget))->ic_attr = NULL;
    }
#endif
}

static gint
gtk_panda_entry_button_press (GtkWidget      *widget,
			      GdkEventButton *event)
{
  if (event->button == 4 || event->button == 5)
    return TRUE;

  return GTK_WIDGET_CLASS (parent_class)->button_press_event (widget, event);
}

static gint
gtk_panda_entry_focus_in (GtkWidget     *widget,
			  GdkEventFocus *event)
{
  GtkEntry *entry;
  GtkEditable *editable;

  g_return_val_if_fail (widget != NULL, FALSE);
  g_return_val_if_fail (GTK_IS_ENTRY (widget), FALSE);
  g_return_val_if_fail (event != NULL, FALSE);

  entry = GTK_ENTRY (widget);
  editable = GTK_EDITABLE (widget);

#ifdef USE_XIM
  if ((GTK_PANDA_ENTRY (widget)->input_mode == GTK_PANDA_ENTRY_XIM_MODE
       || GTK_PANDA_ENTRY (widget)->input_mode == GTK_PANDA_ENTRY_KANA_MODE)
      && editable->ic == NULL
      && gdk_im_ready ()
      && (editable->ic_attr = gdk_ic_attr_new ()) != NULL)
    {
      gint width, height;
      GdkEventMask mask;
      GdkColormap *colormap;
      GdkICAttr *attr = editable->ic_attr;
      GdkICAttributesType attrmask = GDK_IC_ALL_REQ;
      GdkIMStyle style;
      GdkIMStyle supported_style = GDK_IM_PREEDIT_NONE |
				   GDK_IM_PREEDIT_NOTHING |
			           GDK_IM_PREEDIT_POSITION |
			           GDK_IM_STATUS_NONE |
				   GDK_IM_STATUS_NOTHING;

      if (widget->style && widget->style->font->type != GDK_FONT_FONTSET)
	supported_style &= ~GDK_IM_PREEDIT_POSITION;

      attr->style = style = gdk_im_decide_style (supported_style);
      attr->client_window = entry->text_area;

      if ((colormap = gtk_widget_get_colormap (widget)) !=
	    gtk_widget_get_default_colormap ())
	{
	  attrmask |= GDK_IC_PREEDIT_COLORMAP;
	  attr->preedit_colormap = colormap;
	}
      attrmask |= GDK_IC_PREEDIT_FOREGROUND;
      attrmask |= GDK_IC_PREEDIT_BACKGROUND;
      attr->preedit_foreground = widget->style->fg[GTK_STATE_NORMAL];
      attr->preedit_background = widget->style->base[GTK_STATE_NORMAL];

      switch (style & GDK_IM_PREEDIT_MASK)
	{
	case GDK_IM_PREEDIT_POSITION:
	  if (widget->style && widget->style->font->type != GDK_FONT_FONTSET)
	    {
	      g_warning (_("over-the-spot style requires fontset"));
	      break;
	    }

	  gdk_window_get_size (entry->text_area, &width, &height);

	  attrmask |= GDK_IC_PREEDIT_POSITION_REQ;
	  attr->spot_location.x = 0;
	  attr->spot_location.y = height;
	  attr->preedit_area.x = 0;
	  attr->preedit_area.y = 0;
	  attr->preedit_area.width = width;
	  attr->preedit_area.height = height;
	  attr->preedit_fontset = widget->style->font;

	  break;
	}
      editable->ic = gdk_ic_new (attr, attrmask);
     
      if (editable->ic == NULL)
	g_warning (_("Can't create input context."));
      else
	{
	  mask = gdk_window_get_events (entry->text_area);
	  mask |= gdk_ic_get_events (editable->ic);
	  gdk_window_set_events (entry->text_area, mask);

	  if (GTK_WIDGET_HAS_FOCUS(widget))
	    gdk_im_begin (editable->ic, entry->text_area);
  	}
    }
#endif

  if (GTK_WIDGET_CLASS (parent_class)->focus_in_event)
    (* GTK_WIDGET_CLASS (parent_class)->focus_in_event) (widget, event);

#ifdef USE_XIM
  if (GTK_PANDA_ENTRY (widget)->xim_enabled && gdk_xim_ic && gdk_xim_ic->xic)
    {
      XVaNestedList *preedit_attr =
	XVaCreateNestedList (0, XNPreeditState, XIMPreeditEnable, NULL);
      XSetICValues (gdk_xim_ic->xic, XNPreeditAttributes, preedit_attr, NULL);
      XFree (preedit_attr);
    }
#endif

  return FALSE;
}

static gint
gtk_panda_entry_focus_out (GtkWidget     *widget,
			   GdkEventFocus *event)
{
  g_return_val_if_fail (widget != NULL, FALSE);
  g_return_val_if_fail (GTK_IS_ENTRY (widget), FALSE);
  g_return_val_if_fail (event != NULL, FALSE);

  if (GTK_WIDGET_CLASS (parent_class)->focus_out_event)
    (* GTK_WIDGET_CLASS (parent_class)->focus_out_event) (widget, event);

  return FALSE;
}

static gint
gtk_panda_entry_key_press (GtkWidget      *widget,
			GdkEventKey       *event)
{
  GtkPandaEntry *entry;
  gchar *last_char;
  gint start_pos;
  gint end_pos;
  
  entry = GTK_PANDA_ENTRY (widget);

  if (entry->input_mode == GTK_PANDA_ENTRY_KANA_MODE)
    {
      if (event->keyval == GDK_Return)
        {
          start_pos = (GTK_ENTRY(entry)->text_length - 1);
          end_pos = -1;
          last_char = gtk_editable_get_chars(GTK_EDITABLE(entry),start_pos, end_pos);
          if ((last_char != NULL) && (last_char[0] == 'n' ))
           {
              gtk_editable_delete_text (GTK_EDITABLE(entry), start_pos, end_pos);
              gtk_editable_insert_text (GTK_EDITABLE(entry), "ン", strlen("ン"), &start_pos);
              end_pos = start_pos + strlen("ン");
              gtk_editable_set_position(GTK_EDITABLE(entry), end_pos);
            }
          g_free(last_char);
		}
	}
  return TRUE;
}

static gchar *
get_symbol (gchar key)
{
  static gchar *symbol_table[] = {
    "　", "！", "”", "＃", "＄", "％", "＆", "’",
    "（", "）", "＊", "＋", "，", "ー", "．", "／",
    "０", "１", "２", "３", "４", "５", "６", "７",
    "８", "９", "：", "；", "＜", "＝", "＞", "？",
    "＠", "Ａ", "Ｂ", "Ｃ", "Ｄ", "Ｅ", "Ｆ", "Ｇ",
    "Ｈ", "Ｉ", "Ｊ", "Ｋ", "Ｌ", "Ｍ", "Ｎ", "Ｏ",
    "Ｐ", "Ｑ", "Ｒ", "Ｓ", "Ｔ", "Ｕ", "Ｖ", "Ｗ",
    "Ｘ", "Ｙ", "Ｚ", "［", "￥", "］", "＾", "＿",
    "‘", "ア", 0, 0, 0, "エ", 0, 0, 0, "イ", 0, 0, 0, 0, 0, "オ",
    0, 0, 0, 0, 0, "ウ", 0, 0, 0, 0, 0, "｛", "｜", "｝", "￣", 0
  };

  if (key >= 0x20)
    return symbol_table[key - 0x20];
  return 0;
}

static gchar *
get_kana (gchar *prefix, gchar key)
{
  static struct {
    gchar *prefix;
    gchar *a, *i, *u, *e, *o;
  } kana_table[] = {
    {"l",  "ァ", "ィ", "ゥ", "ェ", "ォ"},
    {"x",  "ァ", "ィ", "ゥ", "ェ", "ォ"},
    {"k",  "カ", "キ", "ク", "ケ", "コ"},
    {"ky", "キャ", "キィ", "キュ", "キェ", "キョ"},
    {"g",  "ガ", "ギ", "グ", "ゲ", "ゴ"},
    {"gy", "ギャ", "ギィ", "ギュ", "ギェ", "ギョ"},
    {"s",  "サ", "シ", "ス", "セ", "ソ"},
    {"sh", "シャ", "シ", "シュ", "シェ", "ショ"},
    {"sy", "シャ", "シィ", "シュ", "シェ", "ショ"},
    {"z",  "ザ", "ジ", "ズ", "ゼ", "ゾ"},
    {"j",  "ジャ", "ジ", "ジュ", "ジェ", "ジョ"},
    {"jy", "ジャ", "ジィ", "ジュ", "ジェ", "ジョ"},
    {"zy", "ジャ", "ジィ", "ジュ", "ジェ", "ジョ"},
    {"t",  "タ", "チ", "ツ", "テ", "ト"},
    {"ts", 0, 0, "ツ", 0, 0},
    {"lt", 0, 0, "ッ", 0, 0},
    {"xt", 0, 0, "ッ", 0, 0},
    {"ty", "チャ", "チィ", "チュ", "チェ", "チョ"},
    {"cy", "チャ", "チィ", "チュ", "チェ", "チョ"},
    {"ch", "チャ", "チ", "チュ", "チェ", "チョ"},
    {"th", "テャ", "ティ", "テュ", "テェ", "テョ"},
    {"d",  "ダ", "ヂ", "ヅ", "デ", "ド"},
    {"dy", "ヂャ", "ヂィ", "ヂュ", "ヂェ", "ヂョ"},
    {"dh", "デャ", "ディ", "デュ", "デェ", "デョ"},
    {"dw", "ドァ", "ドィ", "ドゥ", "ドェ", "ドォ"},
    {"n",  "ナ", "ニ", "ヌ", "ネ", "ノ"},
    {"ny", "ニャ", "ニィ", "ニュ", "ニェ", "ニョ"},
    {"h",  "ハ", "ヒ", "フ", "ヘ", "ホ"},
    {"f",  "ファ", "フィ", "フ", "フェ", "フォ"},
    {"hy", "ヒャ", "ヒィ", "ヒュ", "ヒェ", "ヒョ"},
    {"fy", "フャ", "フィ", "フュ", "フェ", "フョ"},
    {"b",  "バ", "ビ", "ブ", "ベ", "ボ"},
    {"by", "ビャ", "ビィ", "ビュ", "ビェ", "ビョ"},
    {"p",  "パ", "ピ", "プ", "ペ", "ポ"},
    {"py", "ピャ", "ピィ", "ピュ", "ピェ", "ピョ"},
    {"m",  "マ", "ミ", "ム", "メ", "モ"},
    {"my", "ミャ", "ミィ", "ミュ", "ミェ", "ミョ"},
    {"y",  "ヤ", "イ", "ユ", "イェ", "ヨ"},
    {"ly", "ャ", 0, "ュ", 0, "ョ"},
    {"xy", "ャ", 0, "ュ", 0, "ョ"},
    {"r",  "ラ", "リ", "ル", "レ", "ロ"},
    {"ry", "リャ", "リィ", "リュ", "リェ", "リョ"},
    {"w",  "ワ", "ヰ", "ウ", "ヱ", "ヲ"},
    {"wh", "ワ", "ウィ", "ウ", "ウェ", "ウォ"},
    {"lw", "ヮ", 0, 0, 0, 0},
    {"xw", "ヮ", 0, 0, 0, 0},
    {"v",  "ヴァ", "ヴィ", "ヴ", "ヴェ", "ヴォ"},
    {"q",  "クァ", "クィ", "ク", "クェ", "クォ"},
    {0, 0, 0, 0, 0, 0}
  };

  int i;
  for (i = 0; kana_table[i].prefix; i++)
    if (strcmp (prefix, kana_table[i].prefix) == 0)
      switch (key)
	{
	case 'a': return kana_table[i].a;
	case 'i': return kana_table[i].i;
	case 'u': return kana_table[i].u;
	case 'e': return kana_table[i].e;
	case 'o': return kana_table[i].o;
	default:  return 0;
	}
  return 0;
}

static void
gtk_panda_entry_insert_text (GtkEditable *editable,
			     const gchar *new_text,
			     gint         new_text_length,
			     gint        *position)
{
  GtkEntry *entry;
  gchar *prefix = NULL;
  
  g_return_if_fail (editable != NULL);
  g_return_if_fail (GTK_IS_EDITABLE (editable));

  entry = GTK_ENTRY (editable);

  /* Handle kana input */
  if (GTK_PANDA_ENTRY (entry)->input_mode == GTK_PANDA_ENTRY_KANA_MODE)
    {
      gchar buff[8];
      gint prefix_start, prefix_end;
      gchar this, last;

      /* Do nothing if there is no input */
      if (new_text_length < 1)
	return;

      /* Just insert for non-keyboard input */
      if (new_text_length > 1)
	goto insert;

      /* Find the prefix */
      prefix_end = gtk_editable_get_position(editable);
      for (prefix_start = prefix_end; prefix_start > 0; prefix_start--)
	{
	  gchar *s = gtk_editable_get_chars (editable, prefix_start - 1, -1);
	  if (*s < 'a' || 'z' < *s)
	    break;
	}
      prefix = gtk_editable_get_chars (editable, prefix_start, prefix_end);

      this = *new_text;
      last = *prefix;

      if (last == 0)
	{
	  /* single char */
	  gchar *s = get_symbol (new_text[0]);
	  if (s != 0)
	    new_text = s;
	  else
	    goto insert;
	}
      else if (this == 'a' || this == 'i' || this == 'u'
	       || this == 'e' || this == 'o')
	{
	  /* カナ */
	  gchar *s = get_kana (prefix, new_text[0]);
	  if (s != 0)
	    new_text = s;
	  else
	    goto insert;
	}
      else if (last == 'n' && this != 'y')
	{
	  /* n -> ン */
	  if (this == 'n' || this == '\'')
	    strcpy (buff, "ン");
	  else
	    sprintf (buff, "ン%c", this);
	  new_text = buff;
	}
      else if (this == last)
	{
	  /* xx -> ッ */
	  sprintf (buff, "ッ%c", this);
	  new_text = buff;
	}
      else
	goto insert;

      new_text_length = strlen (new_text);
      *position = prefix_start;
      gtk_editable_delete_text (editable, prefix_start, prefix_end);
    }

 insert:
  if (GTK_EDITABLE_CLASS (parent_class)->insert_text)
    (* GTK_EDITABLE_CLASS (parent_class)->insert_text)
      (editable, new_text, new_text_length, position);

  g_free(prefix);
}
