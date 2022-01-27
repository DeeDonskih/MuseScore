/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MU_ENGRAVING_OBJECT_H
#define MU_ENGRAVING_OBJECT_H

#include "types.h"
#include "infrastructure/draw/geometry.h"
#include "style/styledef.h"
#include "style/textstyle.h"
#include "style/style.h"

#include "modularity/ioc.h"
#include "diagnostics/iengravingelementsprovider.h"

namespace mu::diagnostics {
class EngravingElementsProvider;
}

namespace Ms {
class EngravingObject;
class MasterScore;
class XmlWriter;
class XmlReader;
class ConnectorInfoReader;
class Measure;
class Staff;
class Part;
class Score;
class MuseScoreView;
class Segment;
class EngravingItem;
class BarLine;
class Articulation;
class Marker;
class Clef;
class KeySig;
class TimeSig;
class TempoText;
class Breath;
class Box;
class HBox;
class VBox;
class TBox;
class FBox;
class ChordRest;
class Slur;
class Tie;
class Glissando;
class GlissandoSegment;
class SystemDivider;
class RehearsalMark;
class Harmony;
class Volta;
class Jump;
class StaffText;
class Ottava;
class Note;
class Chord;
class Rest;
class MMRest;
class LayoutBreak;
class Tremolo;
class System;
class Sticking;
class Lyrics;
class LyricsLine;
class LyricsLineSegment;
class Stem;
class SlurSegment;
class TieSegment;
class OttavaSegment;
class Beam;
class Hook;
class StemSlash;
class Spacer;
class StaffLines;
class Ambitus;
class Bracket;
class InstrumentChange;
class Text;
class TextBase;
class Hairpin;
class HairpinSegment;
class Bend;
class TremoloBar;
class MeasureRepeat;
class Tuplet;
class NoteDot;
class Dynamic;
class InstrumentName;
class DurationElement;
class Accidental;
class TextLine;
class TextLineSegment;
class Pedal;
class PedalSegment;
class LedgerLine;
class ActionIcon;
class VoltaSegment;
class NoteLine;
class Trill;
class TrillSegment;
class Symbol;
class FSymbol;
class Fingering;
class NoteHead;
class FiguredBass;
class StaffState;
class Arpeggio;
class Image;
class ChordLine;
class Slide;
class SlurTieSegment;
class FretDiagram;
class StaffTypeChange;
class MeasureBase;
class Page;
class SystemText;
class PlayTechAnnotation;
class BracketItem;
class Spanner;
class SpannerSegment;
class Lasso;
class BagpipeEmbellishment;
class LineSegment;
class BSymbol;
class TextLineBase;
class TextLineBaseSegment;
class Fermata;

class LetRing;
class LetRingSegment;
class TempoChangeRanged;
class TempoChangeRangedSegment;
class Vibrato;
class VibratoSegment;
class PalmMute;
class PalmMuteSegment;
class MeasureNumber;
class MMRestRange;

class StaffTextBase;

enum class Pid : int;
enum class PropertyFlags : char;

class LinkedObjects;
class EngravingObject;

class EngravingObjectList : public std::list<EngravingObject*>
{
public:

    EngravingObject* at(size_t i) const;
};

class EngravingObject
{
    INJECT_STATIC(engraving, mu::diagnostics::IEngravingElementsProvider, elementsProvider)

    ElementType m_type = ElementType::INVALID;
    EngravingObject* m_parent = nullptr;
    bool m_isParentExplicitlySet = false;
    EngravingObjectList m_children;

    Score* m_score = nullptr;

    static ElementStyle const emptyStyle;

    void doSetParent(EngravingObject* p);
    void doSetScore(Score* sc);
    void moveToDummy();

protected:
    const ElementStyle* _elementStyle {& emptyStyle };
    PropertyFlags* _propertyFlagsList { 0 };
    LinkedObjects* _links            { 0 };
    virtual int getPropertyFlagsIdx(Pid id) const;

    //! NOTE For compatibility reasons, hope, we will remove the need for this method.
    void hack_setType(const ElementType& t) { m_type = t; }

    void addChild(EngravingObject* o);
    void removeChild(EngravingObject* o);

public:
    EngravingObject(const ElementType& type, EngravingObject* parent);
    EngravingObject(const EngravingObject& se);

    virtual ~EngravingObject();

    inline ElementType type() const { return m_type; }
    inline bool isType(ElementType t) const { return t == m_type; }
    const char* name() const;
    virtual QString userName() const;

    EngravingObject* parent() const;
    void setParent(EngravingObject* p);
    EngravingObject* explicitParent() const;
    void resetExplicitParent();

    const EngravingObjectList& children() const { return m_children; }

    // Score Tree functions for scan function
    friend class mu::diagnostics::EngravingElementsProvider;
    virtual EngravingObject* scanParent() const { return m_parent; }
    virtual EngravingObject* scanChild(int n) const { Q_UNUSED(n); return nullptr; }
    virtual int scanChildCount() const { return 0; }
    virtual void scanElements(void* data, void (* func)(void*, EngravingItem*), bool all=true);

    // context
    virtual void setScore(Score* s);
    Score* score() const;
    MasterScore* masterScore() const;
    bool onSameScore(const EngravingObject* other) const;
    const MStyle* style() const;
    QString mscoreVersion() const;

    virtual mu::engraving::PropertyValue getProperty(Pid) const = 0;
    virtual bool setProperty(Pid, const mu::engraving::PropertyValue&) = 0;
    virtual mu::engraving::PropertyValue propertyDefault(Pid) const;
    virtual void resetProperty(Pid id);
    mu::engraving::PropertyValue propertyDefault(Pid pid, TextStyleType tid) const;
    virtual bool sizeIsSpatiumDependent() const { return true; }
    virtual bool offsetIsSpatiumDependent() const { return true; }

    virtual void reset();                       // reset all properties & position to default

    virtual Pid propertyId(const QStringRef& xmlName) const;

    virtual void initElementStyle(const ElementStyle*);
    virtual const ElementStyle* styledProperties() const { return _elementStyle; }

    virtual PropertyFlags* propertyFlagsList() const { return _propertyFlagsList; }
    virtual PropertyFlags propertyFlags(Pid) const;
    bool isStyled(Pid pid) const;
    mu::engraving::PropertyValue styleValue(Pid, Sid) const;

    void setPropertyFlags(Pid, PropertyFlags);

    virtual Sid getPropertyStyle(Pid) const;
    bool readProperty(const QStringRef&, XmlReader&, Pid);
    void readProperty(XmlReader&, Pid);
    bool readStyledProperty(XmlReader& e, const QStringRef& tag);

    virtual void readAddConnector(ConnectorInfoReader* info, bool pasteMode);

    virtual void styleChanged();

    virtual void undoChangeProperty(Pid id, const mu::engraving::PropertyValue&, PropertyFlags ps);
    void undoChangeProperty(Pid id, const mu::engraving::PropertyValue&);
    void undoResetProperty(Pid id);

    void undoPushProperty(Pid);
    void writeProperty(XmlWriter& xml, Pid id) const;
    void writeStyledProperties(XmlWriter&) const;

    QList<EngravingObject*> linkList() const;

    void linkTo(EngravingObject*);
    void unlink();
    bool isLinked(EngravingObject* se = nullptr) const;

    virtual void undoUnlink();
    LinkedObjects* links() const { return _links; }
    void setLinks(LinkedObjects* le) { _links = le; }

    //---------------------------------------------------
    // check type
    //
    // Example for ChordRest:
    //
    //    bool             isChordRest()
    //---------------------------------------------------

#define CONVERT(a, b) \
    bool is##a() const { return type() == ElementType::b; \
    }

    CONVERT(Note,          NOTE)
    CONVERT(Rest,          REST)
    CONVERT(MMRest,        MMREST)
    CONVERT(Chord,         CHORD)
    CONVERT(BarLine,       BAR_LINE)
    CONVERT(Articulation,  ARTICULATION)
    CONVERT(Fermata,       FERMATA)
    CONVERT(Marker,        MARKER)
    CONVERT(Clef,          CLEF)
    CONVERT(KeySig,        KEYSIG)
    CONVERT(TimeSig,       TIMESIG)
    CONVERT(Measure,       MEASURE)
    CONVERT(TempoText,     TEMPO_TEXT)
    CONVERT(Breath,        BREATH)
    CONVERT(HBox,          HBOX)
    CONVERT(VBox,          VBOX)
    CONVERT(TBox,          TBOX)
    CONVERT(FBox,          FBOX)
    CONVERT(Tie,           TIE)
    CONVERT(Slur,          SLUR)
    CONVERT(Glissando,     GLISSANDO)
    CONVERT(GlissandoSegment,     GLISSANDO_SEGMENT)
    CONVERT(SystemDivider, SYSTEM_DIVIDER)
    CONVERT(RehearsalMark, REHEARSAL_MARK)
    CONVERT(Harmony,       HARMONY)
    CONVERT(Volta,         VOLTA)
    CONVERT(Jump,          JUMP)
    CONVERT(Ottava,        OTTAVA)
    CONVERT(LayoutBreak,   LAYOUT_BREAK)
    CONVERT(Segment,       SEGMENT)
    CONVERT(Tremolo,       TREMOLO)
    CONVERT(System,        SYSTEM)
    CONVERT(Lyrics,        LYRICS)
    CONVERT(Stem,          STEM)
    CONVERT(Beam,          BEAM)
    CONVERT(Hook,          HOOK)
    CONVERT(StemSlash,     STEM_SLASH)
    CONVERT(SlurSegment,   SLUR_SEGMENT)
    CONVERT(TieSegment,    TIE_SEGMENT)
    CONVERT(Spacer,        SPACER)
    CONVERT(StaffLines,    STAFF_LINES)
    CONVERT(Ambitus,       AMBITUS)
    CONVERT(Bracket,       BRACKET)
    CONVERT(InstrumentChange, INSTRUMENT_CHANGE)
    CONVERT(StaffTypeChange, STAFFTYPE_CHANGE)
    CONVERT(Hairpin,       HAIRPIN)
    CONVERT(HairpinSegment, HAIRPIN_SEGMENT)
    CONVERT(Bend,          BEND)
    CONVERT(TremoloBar,    TREMOLOBAR)
    CONVERT(MeasureRepeat, MEASURE_REPEAT)
    CONVERT(Tuplet,        TUPLET)
    CONVERT(NoteDot,       NOTEDOT)
    CONVERT(Dynamic,       DYNAMIC)
    CONVERT(InstrumentName, INSTRUMENT_NAME)
    CONVERT(Accidental,    ACCIDENTAL)
    CONVERT(TextLine,      TEXTLINE)
    CONVERT(TextLineSegment,      TEXTLINE_SEGMENT)
    CONVERT(Pedal,         PEDAL)
    CONVERT(PedalSegment,  PEDAL_SEGMENT)
    CONVERT(OttavaSegment, OTTAVA_SEGMENT)
    CONVERT(LedgerLine,    LEDGER_LINE)
    CONVERT(ActionIcon,    ACTION_ICON)
    CONVERT(VoltaSegment,  VOLTA_SEGMENT)
    CONVERT(NoteLine,      NOTELINE)
    CONVERT(Trill,         TRILL)
    CONVERT(TrillSegment,  TRILL_SEGMENT)
    CONVERT(LetRing,       LET_RING)
    CONVERT(LetRingSegment, LET_RING_SEGMENT)
    CONVERT(TempoChangeRanged, TEMPO_RANGED_CHANGE)
    CONVERT(TempoChangeRangedSegment, TEMPO_RANGED_CHANGE_SEGMENT)
    CONVERT(Vibrato,       VIBRATO)
    CONVERT(PalmMute,      PALM_MUTE)
    CONVERT(PalmMuteSegment, PALM_MUTE_SEGMENT)
    CONVERT(VibratoSegment,  VIBRATO_SEGMENT)
    CONVERT(Symbol,        SYMBOL)
    CONVERT(FSymbol,       FSYMBOL)
    CONVERT(Fingering,     FINGERING)
    CONVERT(NoteHead,      NOTEHEAD)
    CONVERT(LyricsLine,    LYRICSLINE)
    CONVERT(LyricsLineSegment, LYRICSLINE_SEGMENT)
    CONVERT(FiguredBass,   FIGURED_BASS)
    CONVERT(StaffState,    STAFF_STATE)
    CONVERT(Arpeggio,      ARPEGGIO)
    CONVERT(Image,         IMAGE)
    CONVERT(ChordLine,     CHORDLINE)
    CONVERT(Slide,         SLIDE)
    CONVERT(FretDiagram,   FRET_DIAGRAM)
    CONVERT(Page,          PAGE)
    CONVERT(Text,          TEXT)
    CONVERT(MeasureNumber, MEASURE_NUMBER)
    CONVERT(MMRestRange,   MMREST_RANGE)
    CONVERT(StaffText,     STAFF_TEXT)
    CONVERT(SystemText,    SYSTEM_TEXT)
    CONVERT(PlayTechAnnotation, PLAYTECH_ANNOTATION)
    CONVERT(BracketItem,   BRACKET_ITEM)
    CONVERT(Score,         SCORE)
    CONVERT(Staff,         STAFF)
    CONVERT(Part,          PART)
    CONVERT(BagpipeEmbellishment, BAGPIPE_EMBELLISHMENT)
    CONVERT(Lasso,         LASSO)
    CONVERT(Sticking,      STICKING)
#undef CONVERT

    virtual bool isEngravingItem() const { return false; }   // overridden in element.h
    bool isRestFamily() const { return isRest() || isMMRest() || isMeasureRepeat(); }
    bool isChordRest() const { return isRestFamily() || isChord(); }
    bool isDurationElement() const { return isChordRest() || isTuplet(); }
    bool isSlurTieSegment() const { return isSlurSegment() || isTieSegment(); }
    bool isSLineSegment() const;
    bool isBox() const { return isVBox() || isHBox() || isTBox() || isFBox(); }
    bool isVBoxBase() const { return isVBox() || isTBox() || isFBox(); }
    bool isMeasureBase() const { return isMeasure() || isBox(); }
    bool isTextBase() const;
    bool isTextLineBaseSegment() const
    {
        return isHairpinSegment()
               || isLetRingSegment()
               || isTempoChangeRangedSegment()
               || isTextLineSegment()
               || isOttavaSegment()
               || isPalmMuteSegment()
               || isPedalSegment()
               || isVoltaSegment()
        ;
    }

    bool isLineSegment() const
    {
        return isGlissandoSegment()
               || isLyricsLineSegment()
               || isTextLineBaseSegment()
               || isTrillSegment()
               || isVibratoSegment()
        ;
    }

    bool isSpannerSegment() const
    {
        return isLineSegment() || isTextLineBaseSegment() || isSlurSegment() || isTieSegment();
    }

    bool isBSymbol() const { return isImage() || isSymbol(); }
    bool isTextLineBase() const
    {
        return isHairpin()
               || isLetRing()
               || isTempoChangeRanged()
               || isNoteLine()
               || isOttava()
               || isPalmMute()
               || isPedal()
               || isTextLine()
               || isVolta()
        ;
    }

    bool isSLine() const
    {
        return isTextLineBase() || isTrill() || isGlissando() || isVibrato();
    }

    bool isSpanner() const
    {
        return isSlur()
               || isTie()
               || isGlissando()
               || isLyricsLine()
               || isTextLineBase()
               || isSLine()
        ;
    }

    bool isStaffTextBase() const
    {
        return isStaffText() || isSystemText() || isPlayTechAnnotation();
    }
};

//---------------------------------------------------
// safe casting of ScoreElement
//
// Example for ChordRest:
//
//    ChordRest* toChordRest(EngravingItem* e)
//---------------------------------------------------

static inline ChordRest* toChordRest(EngravingObject* e)
{
    Q_ASSERT(e == 0 || e->type() == ElementType::CHORD || e->type() == ElementType::REST
             || e->type() == ElementType::MMREST || e->type() == ElementType::MEASURE_REPEAT);
    return (ChordRest*)e;
}

static inline const ChordRest* toChordRest(const EngravingObject* e)
{
    Q_ASSERT(e == 0 || e->type() == ElementType::CHORD || e->type() == ElementType::REST
             || e->type() == ElementType::MMREST || e->type() == ElementType::MEASURE_REPEAT);
    return (const ChordRest*)e;
}

static inline DurationElement* toDurationElement(EngravingObject* e)
{
    Q_ASSERT(e == 0 || e->type() == ElementType::CHORD || e->type() == ElementType::REST
             || e->type() == ElementType::MMREST || e->type() == ElementType::MEASURE_REPEAT
             || e->type() == ElementType::TUPLET);
    return (DurationElement*)e;
}

static inline const DurationElement* toDurationElement(const EngravingObject* e)
{
    Q_ASSERT(e == 0 || e->type() == ElementType::CHORD || e->type() == ElementType::REST
             || e->type() == ElementType::MMREST || e->type() == ElementType::MEASURE_REPEAT
             || e->type() == ElementType::TUPLET);
    return (const DurationElement*)e;
}

static inline Rest* toRest(EngravingObject* e)
{
    Q_ASSERT(!e || e->isRestFamily());
    return (Rest*)e;
}

static inline const Rest* toRest(const EngravingObject* e)
{
    Q_ASSERT(!e || e->isRestFamily());
    return (const Rest*)e;
}

static inline SlurTieSegment* toSlurTieSegment(EngravingObject* e)
{
    Q_ASSERT(e == 0 || e->type() == ElementType::SLUR_SEGMENT || e->type() == ElementType::TIE_SEGMENT);
    return (SlurTieSegment*)e;
}

static inline const SlurTieSegment* toSlurTieSegment(const EngravingObject* e)
{
    Q_ASSERT(e == 0 || e->type() == ElementType::SLUR_SEGMENT || e->type() == ElementType::TIE_SEGMENT);
    return (const SlurTieSegment*)e;
}

static inline const MeasureBase* toMeasureBase(const EngravingObject* e)
{
    Q_ASSERT(e == 0 || e->isMeasure() || e->isVBox() || e->isHBox() || e->isTBox() || e->isFBox());
    return (const MeasureBase*)e;
}

static inline MeasureBase* toMeasureBase(EngravingObject* e)
{
    Q_ASSERT(e == 0 || e->isMeasureBase());
    return (MeasureBase*)e;
}

static inline Box* toBox(EngravingObject* e)
{
    Q_ASSERT(e == 0 || e->isBox());
    return (Box*)e;
}

static inline SpannerSegment* toSpannerSegment(EngravingObject* e)
{
    Q_ASSERT(e == 0 || e->isSpannerSegment());
    return (SpannerSegment*)e;
}

static inline const SpannerSegment* toSpannerSegment(const EngravingObject* e)
{
    Q_ASSERT(e == 0 || e->isSpannerSegment());
    return (const SpannerSegment*)e;
}

static inline BSymbol* toBSymbol(EngravingObject* e)
{
    Q_ASSERT(e == 0 || e->isBSymbol());
    return (BSymbol*)e;
}

static inline TextLineBase* toTextLineBase(EngravingObject* e)
{
    Q_ASSERT(e == 0 || e->isTextLineBase());
    return (TextLineBase*)e;
}

static inline TextBase* toTextBase(EngravingObject* e)
{
    Q_ASSERT(e == 0 || e->isTextBase());
    return (TextBase*)e;
}

static inline const TextBase* toTextBase(const EngravingObject* e)
{
    Q_ASSERT(e == 0 || e->isTextBase());
    return (const TextBase*)e;
}

static inline StaffTextBase* toStaffTextBase(EngravingObject* e)
{
    Q_ASSERT(e == 0 || e->isStaffTextBase());
    return (StaffTextBase*)e;
}

static inline const StaffTextBase* toStaffTextBase(const EngravingObject* e)
{
    Q_ASSERT(e == 0 || e->isStaffTextBase());
    return (const StaffTextBase*)e;
}

#define CONVERT(a)  \
    static inline a* to##a(EngravingObject * e) { Q_ASSERT(e == 0 || e->is##a()); return (a*)e; } \
    static inline const a* to##a(const EngravingObject * e) { Q_ASSERT(e == 0 || e->is##a()); return (const a*)e; }

CONVERT(EngravingItem)
CONVERT(Note)
CONVERT(Chord)
CONVERT(BarLine)
CONVERT(Articulation)
CONVERT(Fermata)
CONVERT(Marker)
CONVERT(Clef)
CONVERT(KeySig)
CONVERT(TimeSig)
CONVERT(Measure)
CONVERT(TempoText)
CONVERT(Breath)
CONVERT(HBox)
CONVERT(VBox)
CONVERT(TBox)
CONVERT(FBox)
CONVERT(Spanner)
CONVERT(Tie)
CONVERT(Slur)
CONVERT(Glissando)
CONVERT(GlissandoSegment)
CONVERT(SystemDivider)
CONVERT(RehearsalMark)
CONVERT(Harmony)
CONVERT(Volta)
CONVERT(Jump)
CONVERT(StaffText)
CONVERT(PlayTechAnnotation)
CONVERT(Ottava)
CONVERT(LayoutBreak)
CONVERT(Segment)
CONVERT(Tremolo)
CONVERT(System)
CONVERT(Lyrics)
CONVERT(Stem)
CONVERT(Beam)
CONVERT(Hook)
CONVERT(StemSlash)
CONVERT(LineSegment)
CONVERT(SlurSegment)
CONVERT(TieSegment)
CONVERT(Spacer)
CONVERT(StaffLines)
CONVERT(Ambitus)
CONVERT(Bracket)
CONVERT(InstrumentChange)
CONVERT(StaffTypeChange)
CONVERT(Text)
CONVERT(MeasureNumber)
CONVERT(MMRestRange)
CONVERT(Hairpin)
CONVERT(HairpinSegment)
CONVERT(Bend)
CONVERT(TremoloBar)
CONVERT(MeasureRepeat)
CONVERT(MMRest)
CONVERT(Tuplet)
CONVERT(NoteDot)
CONVERT(Dynamic)
CONVERT(InstrumentName)
CONVERT(Accidental)
CONVERT(TextLine)
CONVERT(TextLineSegment)
CONVERT(Pedal)
CONVERT(PedalSegment)
CONVERT(OttavaSegment)
CONVERT(LedgerLine)
CONVERT(ActionIcon)
CONVERT(VoltaSegment)
CONVERT(NoteLine)
CONVERT(Trill)
CONVERT(TrillSegment)
CONVERT(LetRing)
CONVERT(LetRingSegment)
CONVERT(TempoChangeRanged)
CONVERT(TempoChangeRangedSegment)
CONVERT(Vibrato)
CONVERT(VibratoSegment)
CONVERT(PalmMute)
CONVERT(PalmMuteSegment)
CONVERT(Symbol)
CONVERT(FSymbol)
CONVERT(Fingering)
CONVERT(NoteHead)
CONVERT(LyricsLine)
CONVERT(LyricsLineSegment)
CONVERT(FiguredBass)
CONVERT(StaffState)
CONVERT(Arpeggio)
CONVERT(Image)
CONVERT(ChordLine)
CONVERT(Slide)
CONVERT(FretDiagram)
CONVERT(Page)
CONVERT(SystemText)
CONVERT(BracketItem)
CONVERT(Staff)
CONVERT(Part)
CONVERT(Lasso)
CONVERT(BagpipeEmbellishment)
CONVERT(Sticking)
#undef CONVERT
}

#endif
