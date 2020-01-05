#include "io-abstract.h"
#include "io-term.h"
#include "logging.h"
#include "spans.h"

void flush_span (span_t& sp)
{
	sp.clear();
}

void flush_spans ()
{
	flush_span(the_wids);
	flush_span(the_hgts);
}

void
set_user_scales (double hs, double ws)
{
	Global->user_height_scale = hs;
	Global->user_width_scale = ws;
	io_repaint ();
}

int
get_scaled_height (CELLREF r)
{
	return ((Global->user_height_scale <= 0.)
			? 1
			:  (int) (get_height (r) * Global->height_scale * Global->user_height_scale));
}

int
get_scaled_width (CELLREF c)
{
	return ((Global->user_width_scale <= 0.)
			? 1
			: (int)(get_width (c) * Global->width_scale * Global->user_width_scale));
}

int get_span(const span_t& span, int ref, int incr, int def)
{
	auto search = span.find(ref);
	if(search != span.end() && search->second !=0)
		return search->second + incr;
	else
		return def;
}

int 
get_width (CELLREF col)
{
	return get_span(the_wids, col, -1, default_width);
}


int 
get_nodef_width (CELLREF col)
{
	return get_span(the_wids, col, 0, 0);

}

void 
set_width (CELLREF col, int wid)
{
	the_wids[col] = wid;
	io_update_width(col, wid);
}

/*
class spanner_c {
	public:
		spanner_c(span_t& span, int lo, int hi) : span(span), lo(lo), hi(hi), n(lo) {};
		unsigned short next(int& pos) {
			unsigned short ret =0;
			while
			get_span(span, n, 0, 0);
			return ret;

		};
	private:
		int lo, hi, n;
		span_t span;
};
*/




span_find_t find_span(span_t& spans, CELLREF lo, CELLREF hi)
{
	span_find_t res;
	for(int i=lo; i<=hi; ++i)
		if(spans.find(i) != spans.end())
			res.dq.emplace_back(std::make_pair(i, spans[i]));
	return res;
}





int next_span(span_find_t& sp, CELLREF& n)
{
	if(sp.dq.empty()) return 0;
	auto [n1, span]  = sp.dq[0];
	sp.dq.pop_front();
	n = n1;
	return span;
}

int 
get_height (CELLREF row)
{
	int incr = - 1 + (Global->display_formula_mode && using_curses);
	return get_span(the_hgts, row, incr, default_height);
}

int 
get_nodef_height (CELLREF row)
{
	return get_span(the_hgts, row, 0, 0);
}

void set_height (CELLREF row, int hgt)
{
	the_hgts[row] = hgt;
}




void shift_spans (span_t&  spans, int over, CELLREF lo, CELLREF hi)
{
	log_debug("lists:cc:TODO:shift_spans: this actually needs to do something");
}
