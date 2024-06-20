#define ALPHA 4.71238898038469
#define BETA 1.5707963267948966
#define RADIUS 25
#define THICK 7
#define MIN_RANGE 0
#define MAX_RANGE 3

float getState(int i) {
  switch (i) {
    case 1: return id(Circuit_1_5).state;
    case 2: return id(Circuit_2).state;
    case 3: return id(Circuit_3).state;
    case 4: return id(Circuit_4).state;
    case 5: return id(Circuit_5).state;
    case 6: return id(Circuit_6_10).state;
    default: return 0;
  }
}

float clipToRange(float x) {
  if (x < MIN_RANGE) return MIN_RANGE;
  if (x > MAX_RANGE) return MAX_RANGE;
  return x;
}

// Indexing from 1
int x[] = {0, 40, 100, 160, 40, 100, 160};
int y[] = {0, 33, 33, 33, 95, 95, 95};

void drawInputGauge(esphome::waveshare_epaper::WaveshareEPaper2P9InV2R2 *it, char name[], float measured, int xc, int yc) {
  it->image(xc-RADIUS, yc-RADIUS, &id(gauge));

  measured = clipToRange(measured);

  float val = (measured - MIN_RANGE) / abs(MAX_RANGE - MIN_RANGE) * ALPHA;        
  int x0 = static_cast<int>(xc + RADIUS * cos(PI / 2 + BETA / 2 + val));
  int y0 = static_cast<int>(yc + RADIUS * sin(PI / 2 + BETA / 2 + val));
  int x1 = static_cast<int>(xc + (RADIUS+THICK) * cos(PI / 2 + BETA / 2 + val + 0.1));
  int y1 = static_cast<int>(yc + (RADIUS+THICK) * sin(PI / 2 + BETA / 2 + val + 0.1));
  int x2 = static_cast<int>(xc + (RADIUS+THICK) * cos(PI / 2 + BETA / 2 + val - 0.1));
  int y2 = static_cast<int>(yc + (RADIUS+THICK) * sin(PI / 2 + BETA / 2 + val - 0.1));
  it->line(x0, y0, x1, y1);
  it->line(x1, y1, x2, y2);
  it->line(x2, y2, x0, y0);

  it->printf(xc, yc, &id(font_gauge), TextAlign::CENTER, name);  
  it->printf(xc, yc + RADIUS*0.75, &id(font_gauge), TextAlign::TOP_CENTER, "%.1fA", measured); 
}

void drawInputGauge(esphome::waveshare_epaper::WaveshareEPaper2P9InV2R2 *it, int i) {
  float measured = getState(i);
  char numberstring[2];
  sprintf(numberstring, "%d", i);
  drawInputGauge(it, numberstring, measured, x[i], y[i]);
}

void drawTotalPowerGauge(esphome::waveshare_epaper::WaveshareEPaper2P9InV2R2 *it) {
  // Power gauge
  float total_alpha = PI; // Defined as the gauge angle in radians (270deg)
  float total_beta = 2*PI - total_alpha;
  int total_radius = 40;              // RADIUS of the gauge in pixels
  int total_thick = 7;    
  
  int xc = 245;
  int yc = 65;
  
  it->image(xc-total_radius, yc-total_radius, &id(gauge_1));
  
  float measured = clipToRange(id(current_power_total).state);
 
  float val = (measured - MIN_RANGE) / abs(MAX_RANGE - MIN_RANGE) * total_alpha;        
  int x0 = static_cast<int>(xc + total_radius * cos(PI / 2 + total_beta / 2 + val));
  int y0 = static_cast<int>(yc + total_radius * sin(PI / 2 + total_beta / 2 + val));
  int x1 = static_cast<int>(xc + (total_radius+total_thick) * cos(PI / 2 + total_beta / 2 + val + 0.1));
  int y1 = static_cast<int>(yc + (total_radius+total_thick) * sin(PI / 2 + total_beta / 2 + val + 0.1));
  int x2 = static_cast<int>(xc + (total_radius+total_thick) * cos(PI / 2 + total_beta / 2 + val - 0.1));
  int y2 = static_cast<int>(yc + (total_radius+total_thick) * sin(PI / 2 + total_beta / 2 + val - 0.1));
  it->line(x0, y0, x1, y1);
  it->line(x1, y1, x2, y2);
  it->line(x2, y2, x0, y0);
  
  it->image(xc-11, yc-22, &id(bolt));   
  it->printf(xc, yc + total_radius/2, &id(font_gauge), TextAlign::BOTTOM_CENTER, "%.1fkW", measured);  
}

void drawHeader(esphome::waveshare_epaper::WaveshareEPaper2P9InV2R2 *it) {
  int x_head = 260;
  int y_head = 2;
  it->strftime(x_head, y_head, &id(font_header), TextAlign::TOP_RIGHT, "%H:%M", id(esptime).now());     

  if (id(wifisignal).has_state()) {
    if (id(wifisignal).state >= -50) {
      // Excellent # mdi-wifi-strength-4 
      it->printf(x_head, y_head, &id(font_icon), TextAlign::TOP_LEFT, "\U0000e1d8");
    } else if (id(wifisignal).state >= -60) {
      // Good # mdi-wifi-strength-3 
      it->printf(x_head, y_head, &id(font_icon), TextAlign::TOP_LEFT, "\U0000ebe1");
    } else if (id(wifisignal).state >= -67) {
      // Fair # mdi-wifi-strength-2 
      it->printf(x_head, y_head, &id(font_icon), TextAlign::TOP_LEFT, "\U0000ebd6");
    } else if (id(wifisignal).state >= -70) {
      // Weak # mdi-wifi-strength-1 
      it->printf(x_head, y_head, &id(font_icon), TextAlign::TOP_LEFT, "\U0000ebe4");
    } else {
      // Unlikely working mdi-wifi-strength-0
      it->printf(x_head, y_head, &id(font_icon), TextAlign::TOP_LEFT, "\U0000f0b0");
    }
  } else {
    it->printf(x_head, y_head, &id(font_icon), TextAlign::TOP_LEFT, "\U000f05aa");
  }
}

void drawFooter(esphome::waveshare_epaper::WaveshareEPaper2P9InV2R2 *it) {
  float measured = id(daily_power_total).state;
  it->printf(290, 85, &id(font_gauge), TextAlign::TOP_RIGHT, "%.0fkWh", measured);  
  it->image(200, 85, &id(power));    
  
  measured = id(cost).state;
  it->image(200, 105, &id(cash));          
  
  it->printf(275, 105, &id(font_gauge), TextAlign::TOP_RIGHT, "%.2f", measured);  
  it->image(275, 105, &id(euro));
}
