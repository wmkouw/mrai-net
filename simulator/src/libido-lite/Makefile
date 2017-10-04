CFLAGS=-O2 -g -pipe -fPIC -DHAVE_X11
LIB=libido.a
OBJS=\
	acrread.o \
	acrutil.o \
	acrwrite.o \
	arg.o \
	argexit.o \
	cntallo.o \
	cntcdg.o \
	cntfill.o \
	cntutil.o \
	dicom.o \
	dcmread.o \
	dcmutil.o \
	dyima.o \
	dyimax.o \
	dyimaxold.o \
	imaallo.o \
	imabasic.o \
	imachekal.o \
	imaclear.o \
	imadrawline.o \
	imafill.o \
	imalabel.o \
	imapart.o \
	imamax.o \
	imawrf.o \
	imazoom.o \
	imagette.o \
	kererr.o \
	kerextern.o \
	kernomfic.o \
	kerprint.o \
	lstallo.o \
	lstbasic.o \
	sigtoima.o \
	seqallo.o \
	sigmm.o \
	sigallo.o \
	sigfft.o \
	sigwrf.o \
	str.o \
	strcomp.o \
	utilcalc.o \
	volallo.o \
	volfft.o \
	volmax.o \
	volmin.o \
	volrdf.o \
	volwrf.o

all: $(LIB)

$(LIB): $(OBJS)
	ar rc $(LIB) $(OBJS)
	ranlib $(LIB)

clean:
	-rm -f core $(LIB) $(OBJS)
